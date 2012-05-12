/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SCANOPTSDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CScanOptsDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ScanOptsDlg.hpp"
#include "NameDlg.hpp"
#include <WCL/StrCvt.hpp>

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

CScanOptsDlg::CScanOptsDlg()
	: CDialog(IDD_OPTS_SCAN)
	, m_nThreads(0)
	, m_bAutoExclude(false)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_INCLUDE,      &m_lbInclude)
		CTRL(IDC_EXCLUDE,      &m_lbExclude)
		CTRL(IDC_THREADS,      &m_ebThreads)
		CTRL(IDC_AUTO_EXCLUDE, &m_ckAutoExclude)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_INC_ADD,    BN_CLICKED, &CScanOptsDlg::OnAddInclude)
		CMD_CTRLMSG(IDC_INC_REMOVE, BN_CLICKED, &CScanOptsDlg::OnRemoveInclude)
		CMD_CTRLMSG(IDC_EXC_ADD,    BN_CLICKED, &CScanOptsDlg::OnAddExclude)
		CMD_CTRLMSG(IDC_EXC_REMOVE, BN_CLICKED, &CScanOptsDlg::OnRemoveExclude)
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

void CScanOptsDlg::OnInitDialog()
{
	const size_t first = 0;

	// Load the existing list of included computers.
	for (size_t i = 0; i < m_astrInclude.Size(); ++i)
		m_lbInclude.Add(m_astrInclude[i]);

	// Select 1st by default.
	if (m_lbInclude.Count() > 0)
		m_lbInclude.CurSel(first);

	// Load the existing list of excluded computers.
	for (size_t i = 0; i < m_astrExclude.Size(); ++i)
		m_lbExclude.Add(m_astrExclude[i]);

	// Select 1st by default.
	if (m_lbExclude.Count() > 0)
		m_lbExclude.CurSel(first);

	// Initialise other options.
	m_ebThreads.Text(CStrCvt::FormatInt(m_nThreads));
	m_ckAutoExclude.Check(m_bAutoExclude);
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	The OK button was pressed.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CScanOptsDlg::OnOk()
{
	// Get and validate the checking options.
	m_nThreads     = CStrCvt::ParseUInt(m_ebThreads.Text());
	m_bAutoExclude = m_ckAutoExclude.IsChecked();

	if ( (m_ebThreads.TextLength() == 0) || (m_nThreads < 1) || (m_nThreads > 1000) )
	{
		AlertMsg(TXT("The number of threads should be between 1 and 1000."));
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

void CScanOptsDlg::OnAddInclude()
{
	const size_t first = 0;

	CNameDlg Dlg;

	Dlg.m_bInclude = true;

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
				m_lbInclude.CurSel(first);
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

void CScanOptsDlg::OnRemoveInclude()
{
	size_t nSel = m_lbInclude.CurSel();

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

void CScanOptsDlg::OnAddExclude()
{
	const size_t first = 0;

	CNameDlg Dlg;

	Dlg.m_bInclude = false;

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
				m_lbExclude.CurSel(first);
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

void CScanOptsDlg::OnRemoveExclude()
{
	size_t nSel = m_lbExclude.CurSel();

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
