/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CHECKJOB.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CCheckJob class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CHECKJOB_HPP
#define CHECKJOB_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/ThreadJob.hpp>
#include "Clocks.hpp"

/******************************************************************************
** 
** The background job used to check the clock on a computer.
**
*******************************************************************************
*/

class CCheckJob : public CThreadJob
{
public:
	//
	// Constructors/Destructor.
	//
	CCheckJob(Clock& clock);
	~CCheckJob();
	
	//
	// Methods.
	//
	virtual void Run();

protected:
	//
	// Members.
	//
	Clock&	m_clock;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // CHECKJOB_HPP
