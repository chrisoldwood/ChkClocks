/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DETECTEDDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CDetectedDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "DetectedDlg.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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

CDetectedDlg::CDetectedDlg()
	: CDialog(IDD_DETECTED)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_PROFILES,	&m_lvProfiles)
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

void CDetectedDlg::OnInitDialog()
{
	ASSERT(m_astrNames.Size() == m_astrFolders.Size());

	// Initialise ListView style.
	m_lvProfiles.InsertColumn(0, "Profile", 100, LVCFMT_LEFT);
	m_lvProfiles.InsertColumn(1, "Folder",  225, LVCFMT_LEFT);
	m_lvProfiles.FullRowSelect(true);

	// Add profiles to grid.
	for (int i = 0; i < m_astrNames.Size(); ++i)
	{
		int n = m_lvProfiles.ItemCount();

		m_lvProfiles.InsertItem(n,    m_astrNames[i]  );
		m_lvProfiles.ItemText  (n, 1, m_astrFolders[i]);
	}
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

bool CDetectedDlg::OnOk()
{
	return true;
}