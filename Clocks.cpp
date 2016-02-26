/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLOCKS.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CClocks class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Clocks.hpp"

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

CClocks::CClocks()
	: CTable(TXT("Clocks"))
{
	// Create the table schema.
	AddColumn(TXT("ID"),        MDCT_IDENTITY, 0,            CColumn::IDENTITY);
	AddColumn(TXT("Computer"),  MDCT_FXDSTR,   COMPUTER_LEN, CColumn::DEFAULTS);
	AddColumn(TXT("Domain"),    MDCT_FXDSTR,   DOMAIN_LEN,   CColumn::DEFAULTS);
	AddColumn(TXT("AbsDiff"),   MDCT_INT,      0,            CColumn::DEFAULTS);
	AddColumn(TXT("RelDiff"),   MDCT_INT,      0,            CColumn::DEFAULTS);
	AddColumn(TXT("ErrorCode"), MDCT_INT,      0,            CColumn::DEFAULTS);
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
	oRow[NTDOMAIN]   = TXT("");
	oRow[ABS_DIFF]   = 0;
	oRow[REL_DIFF]   = 0;
	oRow[ERROR_CODE] = 0;
	
	return oRow;
}
