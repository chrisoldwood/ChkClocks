/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SETUPAPP.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CSetupApp class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// "The" application object.
CSetupApp App;

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

#ifdef _DEBUG
const char* CSetupApp::VERSION      = "v1.0 [Debug]";
#else
const char* CSetupApp::VERSION      = "v1.0";
#endif
const char* CSetupApp::INI_FILE_VER = "1.0";

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

CSetupApp::CSetupApp()
	: CApp(m_AppWnd, m_AppCmds)
	, m_bWinNT(false)
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

CSetupApp::~CSetupApp()
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

bool CSetupApp::OnOpen()
{
	// Set the initial app title.
	m_strTitle = "Setup";

	OSVERSIONINFO oInfo = { 0 };

	// Get Windows version info.
	oInfo.dwOSVersionInfoSize = sizeof(oInfo);

	::GetVersionEx(&oInfo);

	// Is Windows NT line?
	m_bWinNT = (oInfo.dwPlatformId == VER_PLATFORM_WIN32_NT);

	// Read the .ini file version.
	CString strVer = m_oIniFile.ReadString("Version", "Version", "");

	// .ini file missing?
	if (strVer == "")
	{
		FatalMsg("Setup config file 'Setup.ini' is missing or corrupt.");
		return false;
	}

	// .ini file missing?
	if (strVer != INI_FILE_VER)
	{
		FatalMsg("Setup config file 'Setup.ini' is not compatible with this version.");
		return false;
	}

	// Get title and product name.
	m_strTitle   = m_oIniFile.ReadString("Main", "Title",   "Setup"  );
	m_strProduct = m_oIniFile.ReadString("Main", "Product", "Product");

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	m_AppWnd.Centre();
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

bool CSetupApp::OnClose()
{
	return true;
}