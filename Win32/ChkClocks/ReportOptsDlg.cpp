/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		REPORTOPTSDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CReportOptsDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ReportOptsDlg.hpp"
#include "ChkClocksApp.hpp"
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

CReportOptsDlg::CReportOptsDlg()
	: CDialog(IDD_OPTS_REPORT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FMT_FIXED,		&m_rbFmtFixed)
		CTRL(IDC_FMT_VARIABLE,	&m_rbFmtVariable)
		CTRL(IDC_TOLERANCE,		&m_ebTolerance)
		CTRL(IDC_CORRECT,		&m_ckHideCorrect)
		CTRL(IDC_FAILED,		&m_ckHideFailed)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
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

void CReportOptsDlg::OnInitDialog()
{
	// Initialise the report options.
	m_rbFmtFixed.Check(m_nFormat == CChkClocksApp::FMT_FIXED);
	m_rbFmtVariable.Check(m_nFormat != CChkClocksApp::FMT_FIXED);
	m_ebTolerance.Text(CStrCvt::FormatInt(m_nTolerance));
	m_ckHideCorrect.Check(m_bHideCorrect);
	m_ckHideFailed.Check(m_bHideFailed);
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

bool CReportOptsDlg::OnOk()
{
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
