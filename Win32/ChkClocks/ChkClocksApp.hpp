/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CHKCLOCKSAPP.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CChkClocksApp class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CHKCLOCKSAPP_HPP
#define CHKCLOCKSAPP_HPP

/******************************************************************************
** 
** The application class.
**
*******************************************************************************
*/

class CChkClocksApp : public CApp
{
public:
	//
	// Constructors/Destructor.
	//
	CChkClocksApp();
	~CChkClocksApp();

	//
	// Members.
	//
	CAppWnd		m_AppWnd;			// Main window.
	CAppCmds	m_AppCmds;			// Command handler.

	CMDB		m_oMDB;				// Memory database.
	CClocks		m_oClocks;			// Clocks table.

	CStrArray	m_astrInclude;		// List of computers to include.
	CStrArray	m_astrExclude;		// List of computers to exclude.

	CRect		m_rcLastPos;		// Main window position.

	//
	// Class methods.
	//
	static CString FmtDifference(CRow& oRow);

	//
	// Constants.
	//
	static const char* VERSION;

protected:
	//
	// Startup and Shutdown template methods.
	//
	virtual	bool OnOpen();
	virtual	bool OnClose();

	//
	// Preferences.
	//
	CIniFile	m_oIniFile;		// .INI FIle

	//
	// Internal methods.
	//
	void LoadConfig();
	void SaveConfig();

	//
	// Constants.
	//
	static const char* INI_FILE_VER;
};

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// The application object.
extern CChkClocksApp App;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/


#endif //CHKCLOCKSAPP_HPP
