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
	int			m_nThreads;			// Thread pool size.
	int			m_nFormat;			// Difference format.
	int			m_nTolerance;		// Difference tolerance.
	bool		m_bHideCorrect;		// Hide correct clocks?
	bool		m_bHideFailed;		// Hide failed checks?

	CRect		m_rcLastPos;		// Main window position.

	//
	// String formatting methods.
	//
	CString FmtDifference(CRow& oRow);
	CString FmtError(CRow& oRow);

	//
	// Constants.
	//
	static const char* VERSION;
	static const char* INI_FILE_VER;
	static const int   DEF_THREADS;
	static const int   DEF_TOLERANCE;
	static const bool  DEF_HIDE_CORRECT;
	static const bool  DEF_HIDE_FAILED;

	// Format types.
	enum FmtType
	{
		FMT_FIXED,
		FMT_VARIABLE,
	};

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
