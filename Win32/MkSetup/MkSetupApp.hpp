/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MKSETUPAPP.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CMakeSetupApp class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MKSETUPAPP_HPP
#define MKSETUPAPP_HPP

/******************************************************************************
** 
** The application class.
**
*******************************************************************************
*/

class CMakeSetupApp : public CApp
{
public:
	//
	// Constructors/Destructor.
	//
	CMakeSetupApp();
	~CMakeSetupApp();

	//
	// Members
	//
	CAppWnd		m_AppWnd;			// Main window.
	CAppCmds	m_AppCmds;			// Command handler.

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
extern CMakeSetupApp App;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/


#endif //MKSETUPAPP_HPP