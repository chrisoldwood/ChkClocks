/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLOCKS.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CClocks class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CClocks::CClocks(CMDB& oDB)
	: CTable(oDB, "Clocks")
{
	// Create the table schema.
	AddColumn("ID",        MDCT_IDENTITY, 0,            CColumn::IDENTITY);
	AddColumn("Computer",  MDCT_FXDSTR,   COMPUTER_LEN, CColumn::DEFAULTS);
	AddColumn("Domain",    MDCT_FXDSTR,   DOMAIN_LEN,   CColumn::DEFAULTS);
	AddColumn("ClockDiff", MDCT_INT,      0,            CColumn::DEFAULTS);
	AddColumn("ErrorCode", MDCT_INT,      0,            CColumn::DEFAULTS);
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CClocks::~CClocks()
{
}

/******************************************************************************
** Method:		CreateRow()
**
** Description:	Creates a new row and initialises it.
**
** Parameters:	None.
**
** Returns:		The new row.
**
*******************************************************************************
*/

CRow& CClocks::CreateRow()
{
	CRow& oRow = CTable::CreateRow();

//	oRow[ID]         = 
//	oRow[COMPUTER]   =
//	oRow[DOMAIN]     =
//	oRow[CLOCK_DIFF] =
//	oRow[ERROR_CODE] =
	
	return oRow;
}
