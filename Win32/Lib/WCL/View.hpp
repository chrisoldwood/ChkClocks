/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		VIEW.HPP
** COMPONENT:	Windows C++ Library.
** DESCRIPTION:	The CView class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef VIEW_HPP
#define VIEW_HPP

/******************************************************************************
** 
** This is the base class for all View windows.
**
*******************************************************************************
*/

class CView : public CCtrlWnd
{
public:
	//
	// Constructors/Destructor.
	//
	CView(CDoc& rDoc);
	~CView();
	
protected:
	//
	// Members.
	//
	CDoc&	m_Doc;	// The views' data.

	//
	// Window creation template methods.
	//
	virtual void GetClassParams(WNDCLASS& rParams);
	virtual void GetCreateParams(WNDCREATE& rParams);

	//
	// Message processors.
	//
	virtual void OnPaint(CDC& rDC);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //VIEW_HPP