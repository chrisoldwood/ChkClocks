/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FAVFILEDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFavFileDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "FavFileDlg.hpp"

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

CFavFileDlg::CFavFileDlg()
	: CDialog(IDD_FAV_FILE)
	, m_pFavFiles(NULL)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_MOD,  &m_cbMod )
		CTRL(IDC_FILE, &m_ebFile)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_BROWSE, BN_CLICKED, OnBrowse)
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

void CFavFileDlg::OnInitDialog()
{
	ASSERT(m_pFavFiles != NULL);

	// Initialise controls.
	for (int i = 0; i < App.m_oMods.RowCount(); ++i)
		m_cbMod.Add(App.m_oMods[i][CMods::MOD_NAME], &App.m_oMods[i]);

	// Load controls with current settings.
	m_cbMod.CurSel(m_cbMod.FindExact(m_strMod));
	m_ebFile.Text(m_strFile);

	// If editing, disable the mod combo.
	if (!m_strMod.Empty())
		m_cbMod.Enable(false);
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

bool CFavFileDlg::OnOk()
{
	// Validate mod name.
	int nSelMod = m_cbMod.CurSel();

	if (nSelMod == CB_ERR)
	{
		AlertMsg("Please select the mod being configured.");
		m_cbMod.Focus();
		return false;
	}

	CString strMod = m_cbMod.Text();

	if ( (strMod != m_strMod) && (m_pFavFiles->SelectRow(CFavFiles::MOD_NAME, strMod) != NULL) )
	{
		AlertMsg("There is already a file configured for the mod.");
		m_cbMod.Focus();
		return false;
	}

	// Validate mod file.
	if (m_ebFile.TextLength() == 0)
	{
		AlertMsg("Please enter the path to the favourites file.");
		m_ebFile.Focus();
		return false;
	}

	CPath strPath = m_ebFile.Text();

	if (!strPath.Exists())
	{
		AlertMsg("The path to the favourites file is invalid.");
		m_ebFile.Focus();
		return false;
	}

	// Save new settings.
	m_strMod  = strMod;
	m_strFile = strPath;

	return true;
}

/******************************************************************************
** Method:		OnBrowse()
**
** Description:	Browse for the favourites file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CFavFileDlg::OnBrowse()
{
	CRegKey oKey;
	CPath   strDefDir;

	// Try and find the regkey that contains the UT base path.
	if (oKey.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Unreal Technology\\Installed Apps\\UnrealTournament"))
	{
		strDefDir  = oKey.QueryString("Folder", "");
		strDefDir += "System";
	}

	// File extensions.
	static char szExts[] = {	"INI Files (*.ini)\0*.ini\0"
								"All Files (*.*)\0*.*\0"
								"\0\0"							};

	CPath strFile;

	// Show Select File common dialog.
	if (strFile.Select(*this, CPath::SaveFile, szExts, "ini", strDefDir))
		m_ebFile.Text(strFile);
}