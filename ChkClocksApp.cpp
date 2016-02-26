/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CHKCLOCKSAPP.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CChkClocksApp class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ChkClocksApp.hpp"
#include <lm.h>
#include <WCL/Time.hpp>
#include <WCL/StrCvt.hpp>

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
const tchar* CChkClocksApp::VERSION = TXT("v1.2 [Debug]");
#else
const tchar* CChkClocksApp::VERSION = TXT("v1.2");
#endif
const tchar* CChkClocksApp::INI_FILE_VER    = TXT("1.0");
const bool  CChkClocksApp::DEF_AUTO_EXCLUDE = true;
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
	, m_AppWnd(m_MainThread, m_AppCmds)
	, m_AppCmds(m_AppWnd)
	, m_oClocks()
	, m_bAutoExclude(DEF_AUTO_EXCLUDE)
	, m_nThreads(DEF_THREADS)
	, m_nFormat(FMT_FIXED)
	, m_nTolerance(DEF_TOLERANCE)
	, m_bHideCorrect(DEF_HIDE_CORRECT)
	, m_bHideFailed(DEF_HIDE_FAILED)
{
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
	m_strTitle = TXT("Check Clocks");

	// Load settings.
	LoadConfig();

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	if (!m_rcLastPos.Empty())
		m_AppWnd.Move(m_rcLastPos);

	// Show it.
	m_AppWnd.Show(m_iCmdShow);

	m_AppCmds.InitialiseUI();

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
	CString strVer = m_oIniFile.ReadString(TXT("Version"), TXT("Version"), INI_FILE_VER);

	// Read the window pos and size.
	m_rcLastPos.left   = m_oIniFile.ReadInt(TXT("UI"), TXT("Left"),   0);
	m_rcLastPos.top    = m_oIniFile.ReadInt(TXT("UI"), TXT("Top"),    0);
	m_rcLastPos.right  = m_oIniFile.ReadInt(TXT("UI"), TXT("Right"),  0);
	m_rcLastPos.bottom = m_oIniFile.ReadInt(TXT("UI"), TXT("Bottom"), 0);

	// Read the misc settings.
	m_bAutoExclude = m_oIniFile.ReadBool(TXT("Main"), TXT("AutoExclude"), m_bAutoExclude);
	m_nThreads     = m_oIniFile.ReadInt (TXT("Main"), TXT("Threads"),     m_nThreads);
	m_nFormat      = m_oIniFile.ReadInt (TXT("Main"), TXT("Format"),      m_nFormat);
	m_nTolerance   = m_oIniFile.ReadInt (TXT("Main"), TXT("Tolerance"),   m_nTolerance);
	m_bHideCorrect = m_oIniFile.ReadBool(TXT("Main"), TXT("HideCorrect"), m_bHideCorrect);
	m_bHideFailed  = m_oIniFile.ReadBool(TXT("Main"), TXT("HideFailed"),  m_bHideFailed);

	// Load the list of computers to include.
	int nIncCount = m_oIniFile.ReadInt(TXT("Include"), TXT("Count"), 0);

	for (int i = 0; i < nIncCount; ++i)
	{
		CString strEntry, strValue;

		strEntry.Format(TXT("Include[%d]"), i);
		strValue = m_oIniFile.ReadString(TXT("Include"), strEntry, TXT(""));

		// Add if not empty AND unique.
		if ((strValue.Length() > 0) && (m_astrInclude.Find(strValue, true) == -1))
			m_astrInclude.Add(strValue);
	}

	// Load the list of computers to exclude.
	int nExcCount = m_oIniFile.ReadInt(TXT("Exclude"), TXT("Count"), 0);

	for (int i = 0; i < nExcCount; ++i)
	{
		CString strEntry, strValue;

		strEntry.Format(TXT("Exclude[%d]"), i);
		strValue = m_oIniFile.ReadString(TXT("Exclude"), strEntry, TXT(""));

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
	m_oIniFile.WriteString(TXT("Version"), TXT("Version"), INI_FILE_VER);

	// Write the window pos and size.
	m_oIniFile.WriteInt(TXT("UI"), TXT("Left"),   m_rcLastPos.left  );
	m_oIniFile.WriteInt(TXT("UI"), TXT("Top"),    m_rcLastPos.top   );
	m_oIniFile.WriteInt(TXT("UI"), TXT("Right"),  m_rcLastPos.right );
	m_oIniFile.WriteInt(TXT("UI"), TXT("Bottom"), m_rcLastPos.bottom);

	// Write the misc settings.
	m_oIniFile.WriteBool(TXT("Main"), TXT("AutoExclude"), m_bAutoExclude);
	m_oIniFile.WriteInt (TXT("Main"), TXT("Threads"),     m_nThreads);
	m_oIniFile.WriteInt (TXT("Main"), TXT("Format"),      m_nFormat);
	m_oIniFile.WriteInt (TXT("Main"), TXT("Tolerance"),   m_nTolerance);
	m_oIniFile.WriteBool(TXT("Main"), TXT("HideCorrect"), m_bHideCorrect);
	m_oIniFile.WriteBool(TXT("Main"), TXT("HideFailed"),  m_bHideFailed);

	// Save the list of computers to include.
	m_oIniFile.WriteInt(TXT("Include"), TXT("Count"), static_cast<int>(m_astrInclude.Size()));

	for (size_t i = 0; i < m_astrInclude.Size(); ++i)
	{
		CString strEntry;

		strEntry.Format(TXT("Include[%d]"), i);
		m_oIniFile.WriteString(TXT("Include"), strEntry, m_astrInclude[i]);
	}

	// Save the list of computers to exclude.
	m_oIniFile.WriteInt(TXT("Exclude"), TXT("Count"), static_cast<int>(m_astrExclude.Size()));

	for (size_t i = 0; i < m_astrExclude.Size(); ++i)
	{
		CString strEntry;

		strEntry.Format(TXT("Exclude[%d]"), i);
		m_oIniFile.WriteString(TXT("Exclude"), strEntry, m_astrExclude[i]);
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
				str.Format(TXT("%+d s"), nDiff);
			}
			else // (m_nFormat == FMT_VARIABLE)
			{
				// Get sign character.
				tchar cSign = (nDiff >= 0) ? TXT('+') : TXT('-');

				// Calculate off +ve value.
				nDiff = abs(nDiff);

				int nHours, nMins, nSecs;

				// Calculate hours out.
				nHours = nDiff  / WCL::SECS_PER_HOUR;
				nDiff -= nHours * WCL::SECS_PER_HOUR;

				// Calculate minutes out.
				nMins  = nDiff / WCL::SECS_PER_MIN;
				nDiff -= nMins * WCL::SECS_PER_MIN;

				// Calculate seconds out.
				nSecs = nDiff;

				// Format difference.
				if (nHours > 0)
					str.Format(TXT("%c%d h %02d m %02d s"), cSign, nHours, nMins, nSecs);
				else if (nMins > 0)
					str.Format(TXT("%c%d m %02d s"), cSign, nMins, nSecs);
				else
					str.Format(TXT("%c%d s"), cSign, nSecs);
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
		str = CStrCvt::FormatError(oRow[CClocks::ERROR_CODE].GetInt());

	return str;
}
