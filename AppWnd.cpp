/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPWND.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppWnd class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppWnd.hpp"
#include "ChkClocksApp.hpp"

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppWnd::CAppWnd(WCL::IMsgThread& thread, WCL::ICmdController& controller)
	: CDlgFrame(IDR_APPICON, m_AppDlg, false)
	, m_ToolBar(thread, controller)
{
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

CAppWnd::~CAppWnd()
{
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Creates the toolbar and status bars.
**
** Parameters:	rcClient	The client rectangle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnCreate(const CRect& rcClient)
{
	//
	// Create and attach the components.
	//
	m_Accel.LoadRsc(IDR_APPACCEL);
	Accel(&m_Accel);

	m_Menu.LoadRsc(IDR_APPMENU);
	Menu(&m_Menu);

	m_ToolBar.Create(*this, IDC_TOOL_BAR, rcClient);
	ToolBar(&m_ToolBar);

	m_StatusBar.Create(*this, IDC_STATUS_BAR, rcClient);
	StatusBar(&m_StatusBar);

	m_AppDlg.RunModeless(*this);

	// Call base class.
	CDlgFrame::OnCreate(rcClient);
}

/******************************************************************************
** Method:		ProcessMsg()
**
** Description:	Show default status bar text if moving over main window/dialog
**				or its children.
**
** Parameters:	rMsg	The message.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppWnd::ProcessMsg(MSG& rMsg)
{
	// Only interested in mouse moves over the main window/dialog
	if ( (rMsg.message == WM_MOUSEMOVE)
	  && ((rMsg.hwnd == Handle()) || (rMsg.hwnd == m_AppDlg.Handle())
	   || (::GetParent(rMsg.hwnd) == m_AppDlg.Handle())) )
	{
		// Show default status message.
		m_StatusBar.Hint(App.m_strDefStatus);
	}

	// Forward to base class.
	return CDlgFrame::ProcessMsg(rMsg);
}

/******************************************************************************
** Method:		OnClose()
**
** Description:	Window closing.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnClose()
{
	// Fetch windows final placement.
	App.m_rcLastPos = Placement();
}
