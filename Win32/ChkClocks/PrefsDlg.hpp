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
	int			m_nThreads;
	int			m_nFormat;
	int			m_nTolerance;
	bool		m_bHideCorrect;
	bool		m_bHideFailed;

protected:
	//
	// Controls.
	//
	CListBox	m_lbInclude;
	CListBox	m_lbExclude;
	CEditBox	m_ebThreads;
	CRadioBtn	m_rbFmtFixed;
	CRadioBtn	m_rbFmtVariable;
	CEditBox	m_ebTolerance;
	CCheckBox	m_ckHideCorrect;
	CCheckBox	m_ckHideFailed;

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
