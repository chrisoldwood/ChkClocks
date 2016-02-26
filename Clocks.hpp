/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLOCKS.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CClocks class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CLOCKS_HPP
#define CLOCKS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <MDBL/Table.hpp>

/******************************************************************************
** 
** The table used to store the clocks checked.
**
*******************************************************************************
*/

class CClocks : public CTable
{
public:
	//
	// Constructors/Destructor.
	//
	CClocks();
	~CClocks();
	
	//
	// Column indices.
	//
	enum
	{
		ID,
		COMPUTER,
		NTDOMAIN,
		ABS_DIFF,
		REL_DIFF,
		ERROR_CODE,
	};

	//
	// Column lengths.
	//
	enum
	{
		COMPUTER_LEN = MAX_PATH,
		DOMAIN_LEN   = MAX_PATH,
	};

	//
	// Methods (overriden).
	//
	virtual CRow& CreateRow();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // CLOCKS_HPP
