/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SCANOPTSDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CScanOptsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SCANOPTSDLG_HPP
#define SCANOPTSDLG_HPP

/******************************************************************************
** 
** The scanning options dialog.
**
*******************************************************************************
*/

class CScanOptsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CScanOptsDlg();
	
	//
	// Members.
	//
	CStrArray	m_astrInclude;
	CStrArray	m_astrExclude;
	int			m_nThreads;
	bool		m_bAutoExclude;
	
protected:
	//
	// Controls.
	//
	CListBox	m_lbInclude;
	CListBox	m_lbExclude;
	CEditBox	m_ebThreads;
	CCheckBox	m_ckAutoExclude;

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

#endif // SCANOPTSDLG_HPP
