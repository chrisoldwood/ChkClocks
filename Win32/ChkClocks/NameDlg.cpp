/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		NAMEDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CNameDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
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

	if ( (m_strName.Length() < 3) || (m_strName[0] != '\\') || (m_strName[1] != '\\') )
	{
		AlertMsg("Please supply the name in the form: \\\\Name.");
		return false;
	}

	return true;
}
