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
#include <lm.h>

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
	, m_nSortColumn(CClocks::ABS_DIFF)
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
	m_lvGrid.InsertColumn(0, "Computer",   125, LVCFMT_LEFT );
	m_lvGrid.InsertColumn(1, "Domain",     100, LVCFMT_LEFT );
	m_lvGrid.InsertColumn(2, "Difference", 100, LVCFMT_RIGHT);
	m_lvGrid.InsertColumn(3, "Error",      200, LVCFMT_LEFT );
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
		int   nError = oRow[CClocks::ERROR_CODE];
		int   nDiff  = oRow[CClocks::ABS_DIFF];

		// Filter out correct clocks?
		if ( (App.m_bHideCorrect) && (nError == NERR_Success) && (nDiff <= App.m_nTolerance) )
			continue;

		// Filter out failed checks?
		if ( (App.m_bHideFailed) && (nError != NERR_Success) )
			continue;

		int nRow = m_lvGrid.ItemCount();

		// Add to the grid.
		m_lvGrid.InsertItem(nRow,    oRow[CClocks::COMPUTER], &oRow);
		m_lvGrid.ItemText  (nRow, 1, oRow[CClocks::DOMAIN]);
		m_lvGrid.ItemText  (nRow, 2, App.FmtDifference(oRow));
		m_lvGrid.ItemText  (nRow, 3, App.FmtError(oRow));
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

	// Get the table column clicked.
	uint nColumn = GetTableColumn(oMsgHdr.iSubItem);

	// Reverse sort order?
	if (nColumn == m_nSortColumn)
	{
		m_eSortOrder = (m_eSortOrder == CSortColumns::ASC) ? CSortColumns::DESC : CSortColumns::ASC;
	}
	// Change sort column.
	else
	{
		// Default to ascending sort.
		m_nSortColumn = nColumn;
		m_eSortOrder  = CSortColumns::ASC;

		// For difference and error columns
		// use a descending sort.
		if ( (m_nSortColumn == CClocks::ABS_DIFF) || (m_nSortColumn == CClocks::ERROR_CODE) )
			m_eSortOrder  = CSortColumns::DESC;
	}

	RefreshView();

	return 0;
}

/******************************************************************************
** Method:		GetTableColumn()
**
** Description:	Gets the table column that maps onto the grid column.
**
** Parameters:	nGridColumn		The grid column.
**
** Returns:		The table column.
**
*******************************************************************************
*/

int CAppDlg::GetTableColumn(int nGridColumn) const
{
	switch (nGridColumn)
	{
		case 0:		return CClocks::COMPUTER;
		case 1:		return CClocks::DOMAIN;
		case 2:		return CClocks::ABS_DIFF;
		case 3:		return CClocks::ERROR_CODE;
		default:	ASSERT(false);	break;
	}

	return 0;
}
