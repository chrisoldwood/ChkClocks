/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPDLG_HPP
#define APPDLG_HPP

/******************************************************************************
** 
** This is the main application dialog.
**
*******************************************************************************
*/

class CAppDlg : public CMainDlg
{
public:
	//
	// Constructors/Destructor.
	//
	CAppDlg();
	
	//
	// Methods.
	//
	void RefreshView();

	CRow* SelectedRow();

protected:
	//
	// Controls.
	//
	CListView	m_lvGrid;

	//
	// Members.
	//
	uint				m_nSortColumn;
	CSortColumns::Dir	m_eSortOrder;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
    LRESULT OnClickColumn(NMHDR& rMsgHdr);
	LRESULT OnRightClick(NMHDR& rMsgHdr);

	//
	// Internal methods.
	//
	int GetTableColumn(int nGridColumn) const;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CRow* CAppDlg::SelectedRow()
{
	CRow* pRow = NULL;

	if (m_lvGrid.IsSelection())
		pRow = static_cast<CRow*>(m_lvGrid.ItemPtr(m_lvGrid.Selection()));

	return pRow;
}

#endif //APPDLG_HPP
