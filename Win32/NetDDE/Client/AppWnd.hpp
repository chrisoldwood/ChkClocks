/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPWND.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppWnd class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPWND_HPP
#define APPWND_HPP

/******************************************************************************
**
** This is the the applications main window.
**
*******************************************************************************
*/

class CAppWnd : public CDlgFrame
{
public:
	//
	// Constructors/Destructor.
	//
	CAppWnd();
	~CAppWnd();

	//
	// Methods.
	//
	void ShowTrayIcon(bool bShow);

	//
	// Members.
	//
	CAppDlg		m_AppDlg;
	CFrameMenu	m_Menu;
	CStatusBar	m_StatusBar;
	CTrayIcon	m_oTrayIcon;

	//
	// Constants.
	//
	static const char* WNDCLASS_NAME;

protected:
	//
	// Internal members.
	//
	CAccel		m_Accel;

	// Child window IDs.
	enum
	{
		IDC_TOOL_BAR   = 100,
		IDC_STATUS_BAR = 101
	};

	// Tray icon IDs.
	enum
	{
		TRAY_ICON_ID = 100,
	};

	// Tray icon message ID.
	enum { WM_USER_TRAY_NOTIFY = WM_USER };

	//
	// Window creation template methods.
	//
	virtual void GetClassParams(WNDCLASS& rParams);
	virtual void GetCreateParams(WNDCREATE& rParams);

	//
	// Message processors.
	//
	virtual void OnCreate(const CRect& rcClient);
	virtual void OnUserMsg(uint nMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnResize(int iFlag, const CSize& rNewSize);
	virtual void OnFocus();
	virtual bool OnQueryClose();
	virtual void OnClose();

	//
	// Internal methods.
	//
	void Restore();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline void CAppWnd::ShowTrayIcon(bool bShow)
{
	if (bShow)
		m_oTrayIcon.Add(*this, TRAY_ICON_ID, WM_USER_TRAY_NOTIFY, IDI_NET_IDLE, "NetDDE Client");
	else
		m_oTrayIcon.Remove();
}

#endif //APPWND_HPP
