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
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPDLG_HPP
