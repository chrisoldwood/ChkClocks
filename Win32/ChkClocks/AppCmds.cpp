/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppCmds class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "AboutDlg.hpp"
#include "PrefsDlg.hpp"
#include "ProgressDlg.hpp"
#include <lm.h>

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static char szExts[] = {	"Text Files (*.txt)\0*.txt\0"
							"All Files (*.*)\0*.*\0"
							"\0\0"							};

static char szDefExt[] = { "txt" };

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
		CMD_ENTRY(ID_FILE_CHECK,		OnFileCheck,		NULL,				 0)
		CMD_ENTRY(ID_FILE_EXIT,			OnFileExit,			NULL,				-1)
		// Report menu.
		CMD_ENTRY(ID_REPORT_CLIPBOARD,	OnReportClipboard,	NULL,				 7)
		CMD_ENTRY(ID_REPORT_FILE,		OnReportFile,		NULL,				 9)
		CMD_ENTRY(ID_REPORT_PRINT,		OnReportPrint,		NULL,				 8)
		// Options menu.
		CMD_ENTRY(ID_OPTIONS_PREFS,		OnOptionsPrefs,		NULL,				-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_ABOUT,		OnHelpAbout,		NULL,				10)
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

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Checking");
	App.m_AppWnd.Enable(false);

	Dlg.UpdateLabel("Initialising...");

	// Create a thread pool.
	CThreadPool	oThreadPool(App.m_nThreads);

	oThreadPool.Start();

	Dlg.UpdateLabel("Finding Domains...");

	// Find all domains.
	CStrArray astrDomains;

	CNetFinder::FindDomains(astrDomains);

	// Find all domain computers...
	for (int i = 0; (i < astrDomains.Size()) && (!Dlg.Abort()); ++i)
	{
		const char* pszDomain = astrDomains[i];
		CStrArray   astrComputers;
		CString     strProgress;

		// Update progress.
		strProgress.Format("Finding %s Computers...", pszDomain);
		Dlg.UpdateLabel(strProgress);

		CNetFinder::FindComputers(pszDomain, astrComputers);

		// Add all computers to the table.
		for (int j = 0; j < astrComputers.Size(); ++j)
		{
			// Ignore, if on the exclude list.
			if (App.m_astrExclude.Find(astrComputers[j], true) != -1)
				continue;

			CRow& oRow = App.m_oClocks.CreateRow();

			oRow[CClocks::COMPUTER]   = astrComputers[j];
			oRow[CClocks::DOMAIN]     = pszDomain;

			App.m_oClocks.InsertRow(oRow);
		}

		// Check for "Cancel" button.
		App.m_MainThread.ProcessMsgQueue();
	}

	// Append those on the include list.
	for (int i = 0; (i < App.m_astrInclude.Size()) && (!Dlg.Abort()); ++i)
	{
		CRow& oRow = App.m_oClocks.CreateRow();

		oRow[CClocks::COMPUTER] = App.m_astrInclude[i];

		App.m_oClocks.InsertRow(oRow);
	}

	Dlg.InitMeter(App.m_oClocks.RowCount());
	Dlg.UpdateLabel("Checking Clocks...");

	// Start jobs to check all computers.
	for (int i = 0; (i < App.m_oClocks.RowCount()) && (!Dlg.Abort()); ++i)
		oThreadPool.AddJob(new CCheckJob(App.m_oClocks[i]));

	// Wait for jobs to complete OR user to cancel.
	while (oThreadPool.CompletedJobCount() != App.m_oClocks.RowCount())
	{
		Dlg.UpdateMeter(oThreadPool.CompletedJobCount());

		// Check for "Cancel" button.
		App.m_MainThread.ProcessMsgQueue();

		if (Dlg.Abort())
		{
			Dlg.UpdateLabel("Cancelling check...");
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
	catch (CFileException& e)
	{
		// Notify user.
		App.AlertMsg(e.ErrorText());
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
	CSize dmFont = oDC.TextExtents("Wy");

	// Calculate number of pages.
	int nPageSize  = rcRect.Height() / dmFont.cy;
	int nRptLines  = strReport.Count('\n');
	int nPages     = nRptLines / nPageSize;
	int nLineStart = 0;

	// Doesn't end on a page?
	if ((nRptLines % nPageSize) != 0)
		nPages++;

	// Start printing.
	oDC.Start("ChkClocks Report");

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
			int nLineEnd = strReport.Find('\n', nLineStart);

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
	for (int i = 0; i < oRS.Count(); ++i)
	{
		CRow& oRow = oRS[i];

		nComputerWidth = max(nComputerWidth, strlen(oRow[CClocks::COMPUTER]));
		nDomainWidth   = max(nDomainWidth,   strlen(oRow[CClocks::DOMAIN])  );
		nDiffWidth     = max(nDiffWidth,     strlen(App.FmtDifference(oRow)));
		nErrorWidth    = max(nErrorWidth,    strlen(App.FmtError(oRow))     );
	}

	// Generate the report.
	for (int i = 0; i < oRS.Count(); ++i)
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


		strLine.Format("%-*s %-*s %*s %-*s\r\n",
						nComputerWidth, oRow[CClocks::COMPUTER].GetString(),
						nDomainWidth,   oRow[CClocks::DOMAIN].GetString(),
						nDiffWidth,     App.FmtDifference(oRow),
						nErrorWidth,    App.FmtError(oRow));

		strReport += strLine;		
	}

	return strReport;
}

/******************************************************************************
** Method:		OnOptionsPrefs()
**
** Description:	Show the preferences dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnOptionsPrefs()
{
	CPrefsDlg Dlg;

	// Initialise with current settings.
	Dlg.m_astrInclude  = App.m_astrInclude;
	Dlg.m_astrExclude  = App.m_astrExclude;
	Dlg.m_nThreads     = App.m_nThreads;
	Dlg.m_nFormat      = App.m_nFormat;
	Dlg.m_nTolerance   = App.m_nTolerance;
	Dlg.m_bHideCorrect = App.m_bHideCorrect;
	Dlg.m_bHideFailed  = App.m_bHideFailed;

	if (Dlg.RunModal(App.m_AppWnd) == IDOK)
	{
		// Save new settings.
		App.m_astrInclude  = Dlg.m_astrInclude;
		App.m_astrExclude  = Dlg.m_astrExclude;
		App.m_nThreads     = Dlg.m_nThreads;
		App.m_nFormat      = Dlg.m_nFormat;
		App.m_nTolerance   = Dlg.m_nTolerance;
		App.m_bHideCorrect = Dlg.m_bHideCorrect;
		App.m_bHideFailed  = Dlg.m_bHideFailed;
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
