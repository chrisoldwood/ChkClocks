/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPTOOLBAR.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppToolBar class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPTOOLBAR_HPP
#define APPTOOLBAR_HPP

/******************************************************************************
** 
** This is the applications toolbar.
**
*******************************************************************************
*/

class CAppToolBar : public CToolBar
{
public:
	//
	// Constructors/Destructor.
	//
	CAppToolBar();
	~CAppToolBar();
	
	//
	// Members.
	//
	CCmdButton	m_btnCheck;
	CCmdButton	m_btnClpbrd;
	CCmdButton	m_btnFile;
	CCmdButton	m_btnPrint;
	CCmdButton	m_btnHelp;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPTOOLBAR_HPP
