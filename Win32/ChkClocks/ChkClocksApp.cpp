/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CHKCLOCKSAPP.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CChkClocksApp class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include <lm.h>

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// "The" application object.
CChkClocksApp App;

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

#ifdef _DEBUG
const char* CChkClocksApp::VERSION      = "v1.0 [Debug]";
#else
const char* CChkClocksApp::VERSION      = "v1.0";
#endif
const char* CChkClocksApp::INI_FILE_VER     = "1.0";
const int   CChkClocksApp::DEF_THREADS      = 10;
const int   CChkClocksApp::DEF_TOLERANCE    = 1;
const bool  CChkClocksApp::DEF_HIDE_CORRECT = false;
const bool  CChkClocksApp::DEF_HIDE_FAILED  = false;

/******************************************************************************
** Method:		Constructor
**
** Description:	Default constructor.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CChkClocksApp::CChkClocksApp()
	: CApp(m_AppWnd, m_AppCmds)
	, m_oClocks(m_oMDB)
	, m_nThreads(DEF_THREADS)
	, m_nFormat(FMT_FIXED)
	, m_nTolerance(DEF_TOLERANCE)
	, m_bHideCorrect(DEF_HIDE_CORRECT)
	, m_bHideFailed(DEF_HIDE_FAILED)
{
	// Create the MDB.
	m_oMDB.AddTable(m_oClocks);
}

/******************************************************************************
** Method:		Destructor
**
** Description:	Cleanup.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CChkClocksApp::~CChkClocksApp()
{

}

/******************************************************************************
** Method:		OnOpen()
**
** Description:	Initialises the application.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CChkClocksApp::OnOpen()
{
	// Set the app title.
	m_strTitle = "Check Clocks";

	// Load settings.
	LoadConfig();
	
	// Load the toolbar bitmap.
	m_rCmdControl.CmdBitmap().LoadRsc(IDR_APPTOOLBAR);

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	if ( (m_iCmdShow == SW_SHOWNORMAL) && (m_rcLastPos.Empty() == false) )
		m_AppWnd.Move(m_rcLastPos);

	// Show it.
	m_AppWnd.Show(m_iCmdShow);

	// Update UI.
	m_AppCmds.UpdateUI();

	return true;
}

/******************************************************************************
** Method:		OnClose()
**
** Description:	Cleans up the application resources.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CChkClocksApp::OnClose()
{
	// Save settings.
	SaveConfig();

	return true;
}

/******************************************************************************
** Method:		LoadConfig()
**
** Description:	Load the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CChkClocksApp::LoadConfig()
{
	// Read the file version.
	CString strVer = m_oIniFile.ReadString("Version", "Version", INI_FILE_VER);

	// Read the window pos and size.
	m_rcLastPos.left   = m_oIniFile.ReadInt("UI", "Left",   0);
	m_rcLastPos.top    = m_oIniFile.ReadInt("UI", "Top",    0);
	m_rcLastPos.right  = m_oIniFile.ReadInt("UI", "Right",  0);
	m_rcLastPos.bottom = m_oIniFile.ReadInt("UI", "Bottom", 0);

	// Read the misc settings.
	m_nThreads     = m_oIniFile.ReadInt ("Main", "Threads",     m_nThreads);
	m_nFormat      = m_oIniFile.ReadInt ("Main", "Format",      m_nFormat);
	m_nTolerance   = m_oIniFile.ReadInt ("Main", "Tolerance",   m_nTolerance);
	m_bHideCorrect = m_oIniFile.ReadBool("Main", "HideCorrect", m_bHideCorrect);
	m_bHideFailed  = m_oIniFile.ReadBool("Main", "HideFailed",  m_bHideFailed);

	// Load the list of computers to include.
	int nIncCount = m_oIniFile.ReadInt("Include", "Count", 0);

	for (int i = 0; i < nIncCount; ++i)
	{
		CString strEntry, strValue;

		strEntry.Format("Include[%d]", i);
		strValue = m_oIniFile.ReadString("Include", strEntry, "");

		// Add if not empty AND unique.
		if ((strValue.Length() > 0) && (m_astrInclude.Find(strValue, true) == -1))
			m_astrInclude.Add(strValue);
	}

	// Load the list of computers to exclude.
	int nExcCount = m_oIniFile.ReadInt("Exclude", "Count", 0);

	for (i = 0; i < nExcCount; ++i)
	{
		CString strEntry, strValue;

		strEntry.Format("Exclude[%d]", i);
		strValue = m_oIniFile.ReadString("Exclude", strEntry, "");

		// Add if not empty AND unique.
		if ((strValue.Length() > 0) && (m_astrExclude.Find(strValue, true) == -1))
			m_astrExclude.Add(strValue);
	}
}

/******************************************************************************
** Method:		SaveConfig()
**
** Description:	Save the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CChkClocksApp::SaveConfig()
{
	// Write the file version.
	m_oIniFile.WriteString("Version", "Version", INI_FILE_VER);

	// Write the window pos and size.
	m_oIniFile.WriteInt("UI", "Left",   m_rcLastPos.left  );
	m_oIniFile.WriteInt("UI", "Top",    m_rcLastPos.top   );
	m_oIniFile.WriteInt("UI", "Right",  m_rcLastPos.right );
	m_oIniFile.WriteInt("UI", "Bottom", m_rcLastPos.bottom);

	// Write the misc settings.
	m_oIniFile.WriteInt ("Main", "Threads",     m_nThreads);
	m_oIniFile.WriteInt ("Main", "Format",      m_nFormat);
	m_oIniFile.WriteInt ("Main", "Tolerance",   m_nTolerance);
	m_oIniFile.WriteBool("Main", "HideCorrect", m_bHideCorrect);
	m_oIniFile.WriteBool("Main", "HideFailed",  m_bHideFailed);

	// Save the list of computers to include.
	m_oIniFile.WriteInt("Include", "Count", m_astrInclude.Size());

	for (int i = 0; i < m_astrInclude.Size(); ++i)
	{
		CString strEntry;

		strEntry.Format("Include[%d]", i);
		m_oIniFile.WriteString("Include", strEntry, m_astrInclude[i]);
	}

	// Save the list of computers to exclude.
	m_oIniFile.WriteInt("Exclude", "Count", m_astrExclude.Size());

	for (i = 0; i < m_astrExclude.Size(); ++i)
	{
		CString strEntry;

		strEntry.Format("Exclude[%d]", i);
		m_oIniFile.WriteString("Exclude", strEntry, m_astrExclude[i]);
	}
}

/******************************************************************************
** Method:		FmtDifference()
**
** Description:	Formats the clock difference.
**
** Parameters:	oRow	The CClocks row for the computer.
**
** Returns:		The difference.
**
*******************************************************************************
*/

CString CChkClocksApp::FmtDifference(CRow& oRow)
{
	CString str;

	// Check was successful?
	if (oRow[CClocks::ERROR_CODE] == NERR_Success)
	{
		int nDiff = oRow[CClocks::REL_DIFF];

		// Outside "zero" tolerace?
		if (abs(nDiff) > m_nTolerance)
		{
			// Format according to preference.
			if (m_nFormat == FMT_FIXED)
			{
				// Format as seconds with +- sign.
				str.Format("%+d s", nDiff);
			}
			else // (m_nFormat == FMT_VARIABLE)
			{
				// Get sign character.
				char cSign = (nDiff >= 0) ? '+' : '-';

				// Calculate off +ve value.
				nDiff = abs(nDiff);

				int nHours, nMins, nSecs;

				// Calculate hours out.
				nHours = nDiff / SECS_PER_HOUR;
				nDiff -= nHours * SECS_PER_HOUR;

				// Calculate minutes out.
				nMins = nDiff / SECS_PER_MIN;
				nDiff -= nMins * SECS_PER_MIN;

				// Calculate seconds out.
				nSecs = nDiff;

				// Format difference.
				if (nHours > 0)
					str.Format("%c%d h %02d m %02d s", cSign, nHours, nMins, nSecs);
				else if (nMins > 0)
					str.Format("%c%d m %02d s", cSign, nMins, nSecs);
				else
					str.Format("%c%d s", cSign, nSecs);
			}
		}
	}

	return str;
}

/******************************************************************************
** Method:		FmtError()
**
** Description:	Formats the error code if one is set.
**
** Parameters:	oRow	The CClocks row for the computer.
**
** Returns:		The error string.
**
*******************************************************************************
*/

CString CChkClocksApp::FmtError(CRow& oRow)
{
	CString str;

	// Error occurred?
	if (oRow[CClocks::ERROR_CODE] != NERR_Success)
		str = App.FormatError(oRow[CClocks::ERROR_CODE].GetInt());

	return str;
}
