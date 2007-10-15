/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		NAMEDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CNameDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
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

CNameDlg::CNameDlg()
	: CDialog(IDD_INCEXC_NAME)
	, m_bInclude(true)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_NAME,	&m_ebName)
	END_CTRL_TABLE
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

void CNameDlg::OnInitDialog()
{
	// Set title.
	if (m_bInclude)
		Title("Computer Name");
	else
		Title("Computer/Domain Name");
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

bool CNameDlg::OnOk()
{
	m_strName = m_ebName.Text();

	// Validate name.
	if (m_strName.Length() == 0)
	{
		AlertMsg("Please supply the name.");
		return false;
	}

	return true;
}
