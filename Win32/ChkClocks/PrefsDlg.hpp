/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PREFSDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CPrefsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PREFSDLG_HPP
#define PREFSDLG_HPP

/******************************************************************************
** 
** This is the dialog used to edit the preferences.
**
*******************************************************************************
*/

class CPrefsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CPrefsDlg();
	
	//
	// Members.
	//
	CStrArray	m_astrInclude;
	CStrArray	m_astrExclude;

protected:
	//
	// Controls.
	//
	CListBox	m_lbInclude;
	CListBox	m_lbExclude;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	void OnAddInclude();
	void OnRemoveInclude();
	void OnAddExclude();
	void OnRemoveExclude();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //PREFSDLG_HPP
