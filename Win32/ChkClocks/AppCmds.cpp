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
#include <atlconv.h>
#include <lm.h>

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
		CMD_ENTRY(ID_FILE_CHECK,		OnFileCheck,		NULL,				-1)
		CMD_ENTRY(ID_FILE_EXIT,			OnFileExit,			NULL,				-1)
		// Report menu.
		CMD_ENTRY(ID_REPORT_CLIPBOARD,	OnReportClipboard,	NULL,				-1)
		CMD_ENTRY(ID_REPORT_FILE,		OnReportFile,		NULL,				-1)
		CMD_ENTRY(ID_REPORT_PRINT,		OnReportPrint,		NULL,				-1)
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
	Dlg.UpdateLabel("Finding Domains...");
	App.m_AppWnd.Enable(false);

	CStrArray astrDomains;

	// Find all domains.
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
			oRow[CClocks::CLOCK_DIFF] = 0;
			oRow[CClocks::ERROR_CODE] = 0;

			App.m_oClocks.InsertRow(oRow);
		}

		// Check for "Cancel" button.
		App.m_MainThread.ProcessMsgQueue();
	}

	// Append those on the include list.
	for (i = 0; (i < App.m_astrInclude.Size()) && (!Dlg.Abort()); ++i)
	{
		CRow& oRow = App.m_oClocks.CreateRow();

		oRow[CClocks::COMPUTER]   = App.m_astrInclude[i];
		oRow[CClocks::DOMAIN]     = "";
		oRow[CClocks::CLOCK_DIFF] = 0;
		oRow[CClocks::ERROR_CODE] = 0;

		App.m_oClocks.InsertRow(oRow);
	}

	Dlg.InitMeter(App.m_oClocks.RowCount());
	Dlg.UpdateLabel("Checking Clocks...");

	// Check all computers.
	for (i = 0; (i < App.m_oClocks.RowCount()) && (!Dlg.Abort()); ++i)
	{
		CRow&       oRow        = App.m_oClocks[i];
		const char* pszComputer = oRow[CClocks::COMPUTER];
		CString     strProgress;

		// Update progress.
		strProgress.Format("Checking: %s", pszComputer);
		Dlg.UpdateLabel(strProgress);

//		CheckComputer(oRow);
		Dlg.UpdateMeter(i);

		// Check for "Cancel" button.
		App.m_MainThread.ProcessMsgQueue();
	}

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
** Method:		CheckComputer()
**
** Description:	Check the clock on the given computer.
**
** Parameters:	oRow	The table row for the computer.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::CheckComputer(CRow& oRow)
{
	USES_CONVERSION;

	// Get the remote time.
	const char*         paszComputer = oRow[CClocks::COMPUTER];
	const wchar_t*      pwszComputer = A2W(paszComputer);
	LPTIME_OF_DAY_INFO	pTimeInfo    = NULL;
	NET_API_STATUS		nStatus      = NERR_Success;

	nStatus = ::NetRemoteTOD(pwszComputer, (LPBYTE*)&pTimeInfo);

	// Success?
	if ( (nStatus == NERR_Success) && (pTimeInfo != NULL) )
	{
		// Get Local and Remote times and compare.
		time_t tLocal  = time(NULL);
		time_t tRemote = pTimeInfo->tod_elapsedt;
		int    nDiff   = tRemote - tLocal;

		oRow[CClocks::CLOCK_DIFF] = abs(nDiff);
	}
	// Error.
	else 
	{
		oRow[CClocks::CLOCK_DIFF] = -1;
		oRow[CClocks::ERROR_CODE] = (int)nStatus;
	}

	// Cleanup.
	if (pTimeInfo != NULL)
		::NetApiBufferFree(pTimeInfo);
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
** Method:		OnReport()
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
}

/******************************************************************************
** Method:		OnReport()
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
}

/******************************************************************************
** Method:		OnReport()
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
	Dlg.m_astrInclude = App.m_astrInclude;
	Dlg.m_astrExclude = App.m_astrExclude;

	if (Dlg.RunModal(App.m_AppWnd) == IDOK)
	{
		// Save new settings.
		App.m_astrInclude = Dlg.m_astrInclude;
		App.m_astrExclude = Dlg.m_astrExclude;
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
