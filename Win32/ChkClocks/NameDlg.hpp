/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		NAMEDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CNameDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef NAMEDLG_HPP
#define NAMEDLG_HPP

/******************************************************************************
** 
** The dialog used to enter a computer or domain name.
**
*******************************************************************************
*/

class CNameDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CNameDlg();
	
	//
	// Members.
	//
	bool		m_bInclude;
	CString		m_strName;

protected:
	//
	// Controls.
	//
	CEditBox	m_ebName;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // NAMEDLG_HPP
