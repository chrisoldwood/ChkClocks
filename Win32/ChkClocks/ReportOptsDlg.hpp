/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		REPORTOPTSDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CReportOptsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef REPORTOPTSDLG_HPP
#define REPORTOPTSDLG_HPP

/******************************************************************************
** 
** The reporting options dialog.
**
*******************************************************************************
*/

class CReportOptsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CReportOptsDlg();
	
	//
	// Members.
	//
	int		m_nFormat;
	int		m_nTolerance;
	bool	m_bHideCorrect;
	bool	m_bHideFailed;

protected:
	//
	// Controls.
	//
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
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // REPORTOPTSDLG_HPP
