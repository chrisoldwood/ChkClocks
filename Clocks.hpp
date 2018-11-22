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

#include <vector>

////////////////////////////////////////////////////////////////////////////////
//! The type used to store a clock that needs checking.

struct Clock
{
	int		Id;
	tstring	Computer;
	tstring	Domain;
	int		AbsoluteDiff;
	int		RelativeDiff;
	DWORD	ErrorCode;

	Clock();
};

//! The default Clock smart pointer type.
typedef Core::SharedPtr<Clock> ClockPtr;
//! The default Clock collection type.
typedef std::vector<ClockPtr> Clocks;

#endif // CLOCKS_HPP
