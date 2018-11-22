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

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/MainDlg.hpp>
#include <WCL/ListView.hpp>

struct Clock;

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

	Clock* SelectedRow();

	// Data columns.
	enum
	{
		COMPUTER,
		NTDOMAIN,
		CLOCK_DIFF,
		ERROR_CODE,
	};

protected:
	//
	// Controls.
	//
	CListView	m_lvGrid;

	//
	// Members.
	//
	uint	m_nSortColumn;
	bool	m_descendingOrder;

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

inline Clock* CAppDlg::SelectedRow()
{
	Clock* clock = nullptr;

	if (m_lvGrid.IsSelection())
		clock = static_cast<Clock*>(m_lvGrid.ItemPtr(m_lvGrid.Selection()));

	return clock;
}

#endif //APPDLG_HPP
