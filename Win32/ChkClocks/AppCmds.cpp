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
#include <MDBL/WhereCmp.hpp>
#include <MDBL/ResultSet.hpp>

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

CAppCmds::CAppCmds()
{
	// Define the command table.
	DEFINE_CMD_TABLE
		// File menu.
		CMD_ENTRY(ID_FILE_CHECK,		&CAppCmds::OnFileCheck,			NULL,				 0)
		CMD_ENTRY(ID_FILE_EXIT,			&CAppCmds::OnFileExit,			NULL,				-1)
		// Report menu.
		CMD_ENTRY(ID_REPORT_CLIPBOARD,	&CAppCmds::OnReportClipboard,	NULL,				 7)
		CMD_ENTRY(ID_REPORT_FILE,		&CAppCmds::OnReportFile,		NULL,				 9)
		CMD_ENTRY(ID_REPORT_PRINT,		&CAppCmds::OnReportPrint,		NULL,				 8)
		// Options menu.
		CMD_ENTRY(ID_OPTIONS_SCANNING,	&CAppCmds::OnOptionsScan,		NULL,				-1)
		CMD_ENTRY(ID_OPTIONS_REPORTING,	&CAppCmds::OnOptionsReport,		NULL,				-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_ABOUT,		&CAppCmds::OnHelpAbout,			NULL,				10)
		// Context menu.
		CMD_ENTRY(ID_EXCLUDE_COMPUTER,	&CAppCmds::OnExcludeComputer,	NULL,				-1)
		CMD_ENTRY(ID_EXCLUDE_DOMAIN,	&CAppCmds::OnExcludeDomain,		NULL,				-1)
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
	App.m_oClocks.Truncate();

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

			CRow& oRow = App.m_oClocks.CreateRow();

			oRow[CClocks::COMPUTER] = pszComputer;
			oRow[CClocks::NTDOMAIN] = pszDomain;

			App.m_oClocks.InsertRow(oRow);
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
		CRow& oRow = App.m_oClocks.CreateRow();

		oRow[CClocks::COMPUTER] = App.m_astrInclude[i];

		App.m_oClocks.InsertRow(oRow);
	}

	Dlg.InitMeter(App.m_oClocks.RowCount());
	Dlg.UpdateLabel(TXT("Checking Clocks..."));

	// Start jobs to check all computers.
	for (size_t i = 0; (i < App.m_oClocks.RowCount()) && (!Dlg.Abort()); ++i)
	{
		ThreadJobPtr pJob(new CCheckJob(App.m_oClocks[i]));
		oThreadPool.AddJob(pJob);
	}

	// Wait for jobs to complete OR user to cancel.
	while (oThreadPool.CompletedJobCount() != static_cast<size_t>(App.m_oClocks.RowCount()))
	{
		Dlg.UpdateMeter(oThreadPool.CompletedJobCount());

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
		App.m_oClocks.Truncate();

	// Format status bar text.
	if (!Dlg.Abort())
		App.m_strDefStatus.Format(TXT("%d computer(s) checked"), App.m_oClocks.RowCount());

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
	if (App.m_oClocks.RowCount() == 0)
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
	if (App.m_oClocks.RowCount() == 0)
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
		App.AlertMsg(TXT("%s"), e.ErrorText());
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
	if (App.m_oClocks.RowCount() == 0)
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
	int nPageSize  = rcRect.Height() / dmFont.cy;
	int nRptLines  = strReport.Count(TXT('\n'));
	int nPages     = nRptLines / nPageSize;
	int nLineStart = 0;

	// Doesn't end on a page?
	if ((nRptLines % nPageSize) != 0)
		nPages++;

	// Start printing.
	oDC.Start(TXT("ChkClocks Report"));

	// For all pages.
	for (int p = 0; p < nPages; ++p)
	{
		oDC.StartPage();

		// Calculate first line rect.
		CRect rcLine  = rcRect;
		rcLine.bottom = rcLine.top + dmFont.cy;

		// Calculate lines on this page.
		int nFirstLine = p * nPageSize;
		int nLastLine  = nFirstLine + nPageSize;

		// Adjust rows, if last page.
		if (nLastLine > nRptLines)
			nLastLine = nRptLines;

		// For all lines on the page.
		for (int l = nFirstLine; l < nLastLine; ++l)
		{
			// Find the end of the report line.
			int nLineEnd = strReport.Find(TXT('\n'), nLineStart);

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
	if (App.m_oClocks.RowCount() == 0)
		return strReport;

	// Get the report details.
	CResultSet oRS = App.m_oClocks.SelectAll();

	// Sort by difference.
	oRS.OrderBy(CSortColumns(CClocks::ABS_DIFF, CSortColumns::DESC));

	// Report column widths.
	uint nComputerWidth = 0;
	uint nDomainWidth   = 0;
	uint nDiffWidth     = 0;
	uint nErrorWidth    = 0;

	// Calculate report column widths.
	for (size_t i = 0; i < oRS.Count(); ++i)
	{
		CRow& oRow = oRS[i];

		nComputerWidth = max(nComputerWidth, tstrlen(oRow[CClocks::COMPUTER]));
		nDomainWidth   = max(nDomainWidth,   tstrlen(oRow[CClocks::NTDOMAIN]));
		nDiffWidth     = max(nDiffWidth,     tstrlen(App.FmtDifference(oRow)));
		nErrorWidth    = max(nErrorWidth,    tstrlen(App.FmtError(oRow))     );
	}

	// Generate the report.
	for (size_t i = 0; i < oRS.Count(); ++i)
	{
		CString strLine;
		CRow&   oRow   = oRS[i];
		int     nError = oRow[CClocks::ERROR_CODE];
		int     nDiff  = oRow[CClocks::ABS_DIFF];

		// Filter out correct clocks?
		if ( (App.m_bHideCorrect) && (nError == NERR_Success) && (nDiff <= App.m_nTolerance) )
			continue;

		// Filter out failed checks?
		if ( (App.m_bHideFailed) && (nError != NERR_Success) )
			continue;


		strLine.Format(TXT("%-*s %-*s %*s %-*s\r\n"),
						nComputerWidth, oRow[CClocks::COMPUTER].GetString(),
						nDomainWidth,   oRow[CClocks::NTDOMAIN].GetString(),
						nDiffWidth,     App.FmtDifference(oRow),
						nErrorWidth,    App.FmtError(oRow));

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
	CRow* pRow = App.m_AppWnd.m_AppDlg.SelectedRow();

	ASSERT(pRow != NULL);

	const tchar* pszComputer = pRow->Field(CClocks::COMPUTER);

	// Add if not already included.
	if (App.m_astrExclude.Find(pszComputer, true) == -1)
		App.m_astrExclude.Add(pszComputer);

	// Remove from results.
	App.m_oClocks.DeleteRow(*pRow);

	// Redisplay results.
	App.m_AppWnd.m_AppDlg.RefreshView();
}

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
	CRow* pRow = App.m_AppWnd.m_AppDlg.SelectedRow();

	ASSERT(pRow != NULL);

	const tchar* pszDomain = pRow->Field(CClocks::NTDOMAIN);

	// Add if not already included.
	if (App.m_astrExclude.Find(pszDomain, true) == -1)
		App.m_astrExclude.Add(pszDomain);

	// Remove all domain computers from the results.
	App.m_oClocks.DeleteRows(App.m_oClocks.Select(CWhereCmp(CClocks::NTDOMAIN, CWhereCmp::EQUALS, pszDomain)));

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
