/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PREFSDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CPrefsDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "PrefsDlg.hpp"
#include "NameDlg.hpp"

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

CPrefsDlg::CPrefsDlg()
	: CDialog(IDD_PREFS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_INCLUDE,		&m_lbInclude)
		CTRL(IDC_EXCLUDE,		&m_lbExclude)
		CTRL(IDC_THREADS,		&m_ebThreads)
		CTRL(IDC_FMT_FIXED,		&m_rbFmtFixed)
		CTRL(IDC_FMT_VARIABLE,	&m_rbFmtVariable)
		CTRL(IDC_TOLERANCE,		&m_ebTolerance)
		CTRL(IDC_CORRECT,		&m_ckHideCorrect)
		CTRL(IDC_FAILED,		&m_ckHideFailed)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_INC_ADD,    BN_CLICKED, OnAddInclude)
		CMD_CTRLMSG(IDC_INC_REMOVE, BN_CLICKED, OnRemoveInclude)
		CMD_CTRLMSG(IDC_EXC_ADD,    BN_CLICKED, OnAddExclude)
		CMD_CTRLMSG(IDC_EXC_REMOVE, BN_CLICKED, OnRemoveExclude)
	END_CTRLMSG_TABLE
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPrefsDlg::OnInitDialog()
{
	// Load the existing list of included computers.
	for (int i = 0; i < m_astrInclude.Size(); ++i)
		m_lbInclude.Add(m_astrInclude[i]);

	// Select 1st by default.
	if (m_lbInclude.Count() > 0)
		m_lbInclude.CurSel(0);

	// Load the existing list of excluded computers.
	for (int i = 0; i < m_astrExclude.Size(); ++i)
		m_lbExclude.Add(m_astrExclude[i]);

	// Select 1st by default.
	if (m_lbExclude.Count() > 0)
		m_lbExclude.CurSel(0);

	char szValue[100];

	// Initialise the other checking options.
	m_ebThreads.Text(itoa(m_nThreads, szValue, 10));

	// Initialise the report options.
	m_rbFmtFixed.Check(m_nFormat == CChkClocksApp::FMT_FIXED);
	m_rbFmtVariable.Check(m_nFormat != CChkClocksApp::FMT_FIXED);
	m_ebTolerance.Text(itoa(m_nTolerance, szValue, 10));
	m_ckHideCorrect.Check(m_bHideCorrect);
	m_ckHideFailed.Check(m_bHideFailed);
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	User pressed OK.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CPrefsDlg::OnOk()
{
	// Get and validate the checking options.
	m_nThreads = atoi(m_ebThreads.Text());

	if ( (m_ebThreads.TextLength() == 0) || (m_nThreads < 1) || (m_nThreads > 1000) )
	{
		AlertMsg("The number of threads should be between 1 and 1000.");
		return false;
	}

	// Get and validate the report options.
	m_nFormat      = m_rbFmtFixed.IsChecked() ? CChkClocksApp::FMT_FIXED : CChkClocksApp::FMT_VARIABLE;
	m_nTolerance   = atoi(m_ebTolerance.Text());
	m_bHideCorrect = m_ckHideCorrect.IsChecked();
	m_bHideFailed  = m_ckHideFailed.IsChecked();

	if ( (m_ebTolerance.TextLength() == 0) || (m_nTolerance < 0) )
	{
		AlertMsg("The tolerance should be a positive number.");
		return false;
	}


	return true;
}

/******************************************************************************
** Method:		OnAddInclude()
**
** Description:	Add a name to the include list.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPrefsDlg::OnAddInclude()
{
	CNameDlg Dlg;

	// Query the user for a name.
	if (Dlg.RunModal(*this) == IDOK)
	{
		// Name not already added?
		if (m_astrInclude.Find(Dlg.m_strName, true) == LB_ERR)
		{
			// Add to listbox and collection.
			m_lbInclude.Add(Dlg.m_strName);
			m_astrInclude.Add(Dlg.m_strName);

			// Select, if 1st item.
			if (m_lbInclude.Count() == 1)
				m_lbInclude.CurSel(0);
		}
	}
}

/******************************************************************************
** Method:		OnRemoveInclude()
**
** Description:	Remove a name from the include list.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPrefsDlg::OnRemoveInclude()
{
	int nSel = m_lbInclude.CurSel();

	// Ignore, if no selection.
	if (nSel == LB_ERR)
		return;

	CString str = m_lbInclude.Text(nSel);

	// Remove from collection and listbox.
	m_astrInclude.Delete(m_astrInclude.Find(str));
	m_lbInclude.Delete(nSel);

	// Update selection.
	if (nSel == m_lbInclude.Count())
		nSel--;

	m_lbInclude.CurSel(nSel);
}

/******************************************************************************
** Method:		OnAddExclude()
**
** Description:	Add a name to the exclude list.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPrefsDlg::OnAddExclude()
{
	CNameDlg Dlg;

	// Query the user for a name.
	if (Dlg.RunModal(*this) == IDOK)
	{
		// Name not already added?
		if (m_astrExclude.Find(Dlg.m_strName, true) == LB_ERR)
		{
			// Add to listbox and collection.
			m_lbExclude.Add(Dlg.m_strName);
			m_astrExclude.Add(Dlg.m_strName);

			// Select, if 1st item.
			if (m_lbExclude.Count() == 1)
				m_lbExclude.CurSel(0);
		}
	}
}

/******************************************************************************
** Method:		OnRemoveExclude()
**
** Description:	Remove a name from the exclude list.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPrefsDlg::OnRemoveExclude()
{
	int nSel = m_lbExclude.CurSel();

	// Ignore, if no selection.
	if (nSel == LB_ERR)
		return;

	CString str = m_lbExclude.Text(nSel);

	// Remove from collection and listbox.
	m_astrExclude.Delete(m_astrExclude.Find(str));
	m_lbExclude.Delete(nSel);

	// Update selection.
	if (nSel == m_lbExclude.Count())
		nSel--;

	m_lbExclude.CurSel(nSel);
}
