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
const char* CChkClocksApp::INI_FILE_VER = "1.0";

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
** Description:	Formats the clock difference, or an error code if one is set.
**
** Parameters:	oRow	The CClocks row for the computer.
**
** Returns:		The difference or an error string.
**
*******************************************************************************
*/

CString CChkClocksApp::FmtDifference(CRow& oRow)
{
	CString str;
	int     nDifference = oRow[CClocks::CLOCK_DIFF];
	int     nErrorCode  = oRow[CClocks::ERROR_CODE];

	// Check was successful?
	if (nErrorCode == NERR_Success)
	{
		// Format differnence with +- sign.
		str.Format("%d", nDifference);
	}
	else // (nErrorCode != NERR_Success)
	{
		// Format error string.
		switch(nErrorCode)
		{
			case 0x00000005:	str = "Access Denied";				break;
			case 0x00000035:	str = "Unknown Host";				break;
			default:			str.Format("0x%08X", nErrorCode);	break;
		}
	}

	return str;
}
