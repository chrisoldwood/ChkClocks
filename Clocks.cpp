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

Clock::Clock()
	: Id(0)
	, Computer()
	, Domain()
	, AbsoluteDiff()
	, RelativeDiff()
	, ErrorCode()
{
}
