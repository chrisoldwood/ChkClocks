/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

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

CAppDlg::CAppDlg()
	: CMainDlg(IDD_MAIN)
	, m_nSortColumn(CClocks::CLOCK_DIFF)
	, m_eSortOrder(CSortColumns::DESC)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GRID,	&m_lvGrid)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_GRID, LEFT_EDGE, TOP_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE

	DEFINE_CTRLMSG_TABLE
		NFY_CTRLMSG(IDC_GRID, LVN_COLUMNCLICK, OnClickColumn)
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

void CAppDlg::OnInitDialog()
{
	// Set grid style.
//	m_lvGrid.Font(CFont(ANSI_FIXED_FONT));
	m_lvGrid.FullRowSelect(true);
//	m_lvGrid.GridLines(true);

	// Create grid columns.
	m_lvGrid.InsertColumn(0, "Computer",   100, LVCFMT_LEFT );
	m_lvGrid.InsertColumn(1, "Domain",     100, LVCFMT_LEFT );
	m_lvGrid.InsertColumn(2, "Difference", 100, LVCFMT_RIGHT);
}

/******************************************************************************
** Method:		RefreshView()
**
** Description:	Load the grid with the clock details.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::RefreshView()
{
	// Reset grid.
	m_lvGrid.DeleteAllItems();

	// Get the cache details and sort.
	CResultSet oRS = App.m_oClocks.SelectAll();

	oRS.OrderBy(CSortColumns(m_nSortColumn, m_eSortOrder));

	// For all rows.
	for (int i = 0; i < oRS.Count(); ++i)
	{
		CRow& oRow = oRS[i];
		int   nRow = m_lvGrid.ItemCount();

		// Add to the grid.
		m_lvGrid.InsertItem(nRow,    oRow[CClocks::COMPUTER], &oRow);
		m_lvGrid.ItemText  (nRow, 1, oRow[CClocks::DOMAIN]);
		m_lvGrid.ItemText  (nRow, 2, CChkClocksApp::FmtDifference(oRow));
	}
}

/******************************************************************************
** Method:		OnClickColumn()
**
** Description:	Grid column clicked, change sort column or reverse sort order.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnClickColumn(NMHDR& rMsgHdr)
{
	NMLISTVIEW& oMsgHdr = reinterpret_cast<NMLISTVIEW&>(rMsgHdr);

	// Get the column clicked.
	uint nColumn = oMsgHdr.iSubItem;

	// Reverse sort order?
	if (nColumn == m_nSortColumn)
	{
		m_eSortOrder = (m_eSortOrder == CSortColumns::ASC) ? CSortColumns::DESC : CSortColumns::ASC;
		RefreshView();
	}
	// Change sort column.
	else
	{
		// Default to ascending sort.
		m_nSortColumn = nColumn;
		m_eSortOrder  = CSortColumns::ASC;

		// For difference column, use descending sort.
		if (m_nSortColumn == CClocks::CLOCK_DIFF)
			m_eSortOrder  = CSortColumns::DESC;
	}

	return 0;
}
