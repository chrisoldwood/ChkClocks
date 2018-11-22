/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppCmds class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppCmds.hpp"
#include "ChkClocksApp.hpp"
#include "AboutDlg.hpp"
#include "ScanOptsDlg.hpp"
#include "ReportOptsDlg.hpp"
#include "ProgressDlg.hpp"
#include "CheckJob.hpp"
#include <lm.h>
#include <WCL/BusyCursor.hpp>
#include <WCL/ThreadPool.hpp>
#include <WCL/NetFinder.hpp>
#include <WCL/Clipboard.hpp>
#include <WCL/File.hpp>
#include <WCL/FileException.hpp>
#include <WCL/Printer.hpp>
#include <WCL/PrinterDC.hpp>
#include <Core/Algorithm.hpp>
#include <Core/Functor.hpp>
#include <algorithm>

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static tchar szExts[] = {	TXT("Text Files (*.txt)\0*.txt\0")
							TXT("All Files (*.*)\0*.*\0")
							TXT("\0\0")							};

static tchar szDefExt[] = { TXT("txt") };

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppCmds::CAppCmds(CAppWnd& appWnd)
	: CCmdControl(appWnd, IDR_APPTOOLBAR)
{
	// Define the command table.
	DEFINE_CMD_TABLE
		// File menu.
		CMD_ENTRY(ID_FILE_CHECK,		&CAppCmds::OnFileCheck,			nullptr,				 0)
		CMD_ENTRY(ID_FILE_EXIT,			&CAppCmds::OnFileExit,			nullptr,				-1)
		// Report menu.
		CMD_ENTRY(ID_REPORT_CLIPBOARD,	&CAppCmds::OnReportClipboard,	nullptr,				 7)
		CMD_ENTRY(ID_REPORT_FILE,		&CAppCmds::OnReportFile,		nullptr,				 9)
		CMD_ENTRY(ID_REPORT_PRINT,		&CAppCmds::OnReportPrint,		nullptr,				 8)
		// Options menu.
		CMD_ENTRY(ID_OPTIONS_SCANNING,	&CAppCmds::OnOptionsScan,		nullptr,				-1)
		CMD_ENTRY(ID_OPTIONS_REPORTING,	&CAppCmds::OnOptionsReport,		nullptr,				-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_ABOUT,		&CAppCmds::OnHelpAbout,			nullptr,				10)
		// Context menu.
		CMD_ENTRY(ID_EXCLUDE_COMPUTER,	&CAppCmds::OnExcludeComputer,	nullptr,				-1)
		CMD_ENTRY(ID_EXCLUDE_DOMAIN,	&CAppCmds::OnExcludeDomain,		nullptr,				-1)
	END_CMD_TABLE
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppCmds::~CAppCmds()
{
}

/******************************************************************************
** Method:		OnFileCheck()
**
** Description:	Scan the network getting the clock times.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileCheck()
{
	CBusyCursor oCursor;

	// Trash old table.
	App.m_oClocks.clear();

	App.m_strDefStatus = TXT("");

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title(TXT("Checking"));
	App.m_AppWnd.Enable(false);

	Dlg.UpdateLabel(TXT("Initialising..."));

	// Create a thread pool.
	CThreadPool	oThreadPool(App.m_nThreads);

	oThreadPool.Start();

	Dlg.UpdateLabel(TXT("Finding Domains..."));

	// Find all domains.
	CStrArray astrDomains;

	CNetFinder::FindDomains(astrDomains);

	// Find all domain computers...
	for (size_t i = 0; (i < astrDomains.Size()) && (!Dlg.Abort()); ++i)
	{
		const tchar* pszDomain = astrDomains[i];
		CStrArray   astrComputers;
		CString     strProgress;

		// Ignore, if on the exclude list.
		if (App.m_astrExclude.Find(pszDomain, true) != -1)
			continue;

		// Update progress.
		strProgress.Format(TXT("Finding %s Computers..."), pszDomain);
		Dlg.UpdateLabel(strProgress);

		CNetFinder::FindComputers(pszDomain, astrComputers);

		// Add all computers to the table.
		for (size_t j = 0; j < astrComputers.Size(); ++j)
		{
			const tchar* pszComputer = astrComputers[j];

			// Ignore, if on the exclude list.
			if (App.m_astrExclude.Find(pszComputer, true) != -1)
				continue;

			ClockPtr clock(new Clock);

			clock->Computer = pszComputer;
			clock->Domain = pszDomain;

			App.m_oClocks.push_back(clock);
		}

		// Exclude domain, if domain empty AND auto-exlude is on.
		if ( (astrComputers.Size() == 0) && (App.m_bAutoExclude) )
			App.m_astrExclude.Add(pszDomain);

		// Check for "Cancel" button.
		App.m_MainThread.ProcessMsgQueue();
	}

	// Append those on the include list.
	for (size_t i = 0; (i < App.m_astrInclude.Size()) && (!Dlg.Abort()); ++i)
	{
		ClockPtr clock(new Clock);

		clock->Computer = App.m_astrInclude[i];

		App.m_oClocks.push_back(clock);
	}

	Dlg.InitMeter(App.m_oClocks.size());
	Dlg.UpdateLabel(TXT("Checking Clocks..."));

	// Start jobs to check all computers.
	for (size_t i = 0; (i < App.m_oClocks.size()) && (!Dlg.Abort()); ++i)
	{
		ThreadJobPtr pJob(new CCheckJob(App.m_oClocks[i].getRef()));
		oThreadPool.AddJob(pJob);
	}

	// Wait for jobs to complete OR user to cancel.
	while (oThreadPool.CompletedJobCount() != App.m_oClocks.size())
	{
		Dlg.UpdateMeter(static_cast<uint>(oThreadPool.CompletedJobCount()));

		// Check for "Cancel" button.
		App.m_MainThread.ProcessMsgQueue();

		if (Dlg.Abort())
		{
			Dlg.UpdateLabel(TXT("Cancelling check..."));
			oThreadPool.CancelAllJobs();
		}

		Sleep(250);
	}

	// Stop the thread pool.
	oThreadPool.DeleteCompletedJobs();
	oThreadPool.Stop();

	// Remove progress dialog.
	App.m_AppWnd.Enable(true);
	Dlg.Destroy();

	// Trash table, if aborted.
	if (Dlg.Abort())
		App.m_oClocks.clear();

	// Format status bar text.
	if (!Dlg.Abort())
		App.m_strDefStatus.Format(TXT("%d computer(s) checked"), App.m_oClocks.size());

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
}

/******************************************************************************
** Method:		OnFileExit()
**
** Description:	Terminate the app.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileExit()
{
	App.m_AppWnd.Close();
}

/******************************************************************************
** Method:		OnReportClipboard()
**
** Description:	Send the report to the clipboard.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnReportClipboard()
{
	// Ignore if nothing to report.
	if (App.m_oClocks.empty())
		return;

	CBusyCursor oCursor;

	// Generate it.
	CString strReport = GenerateReport();

	// Send to clipboard.
	CClipboard::CopyText(App.m_AppWnd.Handle(), strReport);
}

/******************************************************************************
** Method:		OnReportFile()
**
** Description:	Send the report to a file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnReportFile()
{
	// Ignore if nothing to report.
	if (App.m_oClocks.empty())
		return;

	CPath strFileName;

	// Get the file to save to.
	if (!strFileName.Select(App.m_AppWnd, CPath::SaveFile, szExts, szDefExt, CPath::ApplicationDir()))
		return;

	CBusyCursor oCursor;

	// Generate it.
	CString strReport = GenerateReport();

	try
	{
		CFile oFile;

		// Save it.
		oFile.Create(strFileName);
		oFile.Write(strReport, strReport.Length());
		oFile.Close();
	}
	catch (const CFileException& e)
	{
		// Notify user.
		App.AlertMsg(TXT("%s"), e.twhat());
	}
}

/******************************************************************************
** Method:		OnReportPrint()
**
** Description:	Send the report to a printer.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnReportPrint()
{
	// Ignore if nothing to report.
	if (App.m_oClocks.empty())
		return;

	CPrinter oPrinter;

	// Select a printer.
	if (!oPrinter.Select(App.m_AppWnd))
		return;

	CBusyCursor oCursor;

	// Generate it.
	CString strReport = GenerateReport();

	// Create the printer DC.
	CPrinterDC oDC(oPrinter);

	// Get printer attributes.
	CRect rcRect = oDC.PrintableArea();
	CSize dmFont = oDC.TextExtents(TXT("Wy"));

	// Calculate number of pages.
	size_t nPageSize  = rcRect.Height() / dmFont.cy;
	size_t nRptLines  = strReport.Count(TXT('\n'));
	size_t nPages     = nRptLines / nPageSize;
	size_t nLineStart = 0;

	// Doesn't end on a page?
	if ((nRptLines % nPageSize) != 0)
		nPages++;

	// Start printing.
	oDC.Start(TXT("ChkClocks Report"));

	// For all pages.
	for (size_t p = 0; p < nPages; ++p)
	{
		oDC.StartPage();

		// Calculate first line rect.
		CRect rcLine  = rcRect;
		rcLine.bottom = rcLine.top + dmFont.cy;

		// Calculate lines on this page.
		size_t nFirstLine = p * nPageSize;
		size_t nLastLine  = nFirstLine + nPageSize;

		// Adjust rows, if last page.
		if (nLastLine > nRptLines)
			nLastLine = nRptLines;

		// For all lines on the page.
		for (size_t l = nFirstLine; l < nLastLine; ++l)
		{
			// Find the end of the report line.
			size_t nLineEnd = strReport.Find(TXT('\n'), nLineStart);

			// Extract report line.
			CString strLine = strReport.Mid(nLineStart, nLineEnd-nLineStart-1);

			// Print it.
			oDC.DrawText(rcLine, strLine, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

			// Update rect to start of next line.
			rcLine.top    = rcLine.bottom;
			rcLine.bottom = rcLine.top + dmFont.cy;

			// Move to the next line.
			nLineStart = nLineEnd + 1;
		}

		oDC.EndPage();
	}

	// Done printing.
	oDC.End();
}

////////////////////////////////////////////////////////////////////////////////
//! Comparison function for order by the absolute clock difference.

static bool ClockDifferenceDescending(ClockPtr lhs, ClockPtr rhs)
{
	return rhs->AbsoluteDiff < lhs->AbsoluteDiff;
}

/******************************************************************************
** Method:		GenerateReport()
**
** Description:	Generate the report.
**
** Parameters:	None.
**
** Returns:		The report.
**
*******************************************************************************
*/

CString CAppCmds::GenerateReport()
{
	CString strReport;

	// Ignore if nothing to report.
	if (App.m_oClocks.empty())
		return strReport;

	// Get the report details.
	Clocks clocks = App.m_oClocks;

	// Sort by difference.
	std::sort(clocks.begin(), clocks.end(), ClockDifferenceDescending);

	// Report column widths.
	uint nComputerWidth = 0;
	uint nDomainWidth   = 0;
	uint nDiffWidth     = 0;
	uint nErrorWidth    = 0;

	// Calculate report column widths.
	for (size_t i = 0; i < clocks.size(); ++i)
	{
		Clock& clock = clocks[i].getRef();

		nComputerWidth = std::max(nComputerWidth, static_cast<uint>(clock.Computer.length()));
		nDomainWidth   = std::max(nDomainWidth,   static_cast<uint>(clock.Domain.length()));
		nDiffWidth     = std::max(nDiffWidth,     static_cast<uint>(App.FmtDifference(clock).Length()));
		nErrorWidth    = std::max(nErrorWidth,    static_cast<uint>(App.FmtError(clock).Length()));
	}

	// Generate the report.
	for (size_t i = 0; i < clocks.size(); ++i)
	{
		CString strLine;
		Clock&  clock  = clocks[i].getRef();
		DWORD   nError = clock.ErrorCode;
		int     nDiff  = clock.AbsoluteDiff;

		// Filter out correct clocks?
		if ( (App.m_bHideCorrect) && (nError == NERR_Success) && (nDiff <= App.m_nTolerance) )
			continue;

		// Filter out failed checks?
		if ( (App.m_bHideFailed) && (nError != NERR_Success) )
			continue;


		strLine.Format(TXT("%-*s %-*s %*s %-*s\r\n"),
						nComputerWidth, clock.Computer.c_str(),
						nDomainWidth,   clock.Domain.c_str(),
						nDiffWidth,     App.FmtDifference(clock).c_str(),
						nErrorWidth,    App.FmtError(clock).c_str());

		strReport += strLine;		
	}

	return strReport;
}

/******************************************************************************
** Method:		OnOptionsScan()
**
** Description:	Show the scanning options dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnOptionsScan()
{
	CScanOptsDlg Dlg;

	// Initialise with current settings.
	Dlg.m_astrInclude  = App.m_astrInclude;
	Dlg.m_astrExclude  = App.m_astrExclude;
	Dlg.m_nThreads     = App.m_nThreads;
	Dlg.m_bAutoExclude = App.m_bAutoExclude;

	if (Dlg.RunModal(App.m_AppWnd) == IDOK)
	{
		// Save new settings.
		App.m_astrInclude  = Dlg.m_astrInclude;
		App.m_astrExclude  = Dlg.m_astrExclude;
		App.m_nThreads     = Dlg.m_nThreads;
		App.m_bAutoExclude = Dlg.m_bAutoExclude;
	}
}

/******************************************************************************
** Method:		OnOptionsReport()
**
** Description:	Show the reporting options dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnOptionsReport()
{
	CReportOptsDlg Dlg;

	// Initialise with current settings.
	Dlg.m_nFormat      = App.m_nFormat;
	Dlg.m_nTolerance   = App.m_nTolerance;
	Dlg.m_bHideCorrect = App.m_bHideCorrect;
	Dlg.m_bHideFailed  = App.m_bHideFailed;

	if (Dlg.RunModal(App.m_AppWnd) == IDOK)
	{
		// Save new settings.
		App.m_nFormat      = Dlg.m_nFormat;
		App.m_nTolerance   = Dlg.m_nTolerance;
		App.m_bHideCorrect = Dlg.m_bHideCorrect;
		App.m_bHideFailed  = Dlg.m_bHideFailed;

		// Redisplay results.
		App.m_AppWnd.m_AppDlg.RefreshView();
	}
}

/******************************************************************************
** Method:		OnHelpAbout()
**
** Description:	Show the about dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnHelpAbout()
{
	CAboutDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

////////////////////////////////////////////////////////////////////////////////
//! Predicate for comparing two Clock pointers: one raw, one smart.

CORE_DEFINE_PREDICATE_EX(ClockEquals, Clock*, Clock*, value, ClockPtr, rhs)
{
	return rhs.get() == value;
}
CORE_END_PREDICATE_EX

/******************************************************************************
** Method:		OnExcludeComputer()
**
** Description:	Add the selected computer to the exclusions.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnExcludeComputer()
{
	// Get the selected computer.
	Clock* clock = App.m_AppWnd.m_AppDlg.SelectedRow();

	ASSERT(clock != nullptr);

	// Add if not already included.
	if (App.m_astrExclude.Find(clock->Computer.c_str(), true) == -1)
		App.m_astrExclude.Add(clock->Computer.c_str());

	// Remove from results.
	Core::erase_if(App.m_oClocks, ClockEquals(clock));

	// Redisplay results.
	App.m_AppWnd.m_AppDlg.RefreshView();
}

////////////////////////////////////////////////////////////////////////////////
//! Predicate for comparing two 

CORE_DEFINE_PREDICATE_EX(DomainEquals, tstring, const tstring&, value, ClockPtr, rhs)
{
	return rhs->Domain == value;
}
CORE_END_PREDICATE_EX

/******************************************************************************
** Method:		OnHelpAbout()
**
** Description:	Add the selected domain to the exclusions.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnExcludeDomain()
{
	// Get the selected domain.
	Clock* clock = App.m_AppWnd.m_AppDlg.SelectedRow();

	ASSERT(clock != nullptr);

	// Add if not already included.
	if (App.m_astrExclude.Find(clock->Domain.c_str(), true) == -1)
		App.m_astrExclude.Add(clock->Domain.c_str());

	// Remove all domain computers from the results.
	Core::erase_if(App.m_oClocks, DomainEquals(clock->Domain));

	// Redisplay results.
	App.m_AppWnd.m_AppDlg.RefreshView();
}

/******************************************************************************
** Method:		OnUI...()
**
** Description:	UI update handlers.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/
