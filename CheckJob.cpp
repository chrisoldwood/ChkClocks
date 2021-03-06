/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CHECKJOB.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CCheckJob class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "CheckJob.hpp"
#include "Clocks.hpp"
#include <lm.h>
#include <Core/AnsiWide.hpp>
#include <time.h>
// declaration of 'null' hides global declaration in <MDBL\MDBLTypes.hpp>
#pragma warning ( disable : 4459 )
#include <Core/Scoped.hpp>
#pragma warning ( default : 4459 )

#ifndef _DEBUG
#define	ATLASSERT
#endif

// Add .lib to linker.
#pragma comment(lib, "netapi32")

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

CCheckJob::CCheckJob(Clock& clock)
	: m_clock(clock)
{
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

CCheckJob::~CCheckJob()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Wrapper to use as the deleter for the buffer because NetApiBufferFree()
//! doesn't return void.

static void FreeTimeOfDayInfo(TIME_OF_DAY_INFO* buffer)
{
	::NetApiBufferFree(buffer);
}

/******************************************************************************
** Method:		Run()
**
** Description:	Check the clock on the computer.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CCheckJob::Run()
{
	typedef Core::Scoped<TIME_OF_DAY_INFO*> TimeOfDayInfoPtr;

	// Get the remote time.
	std::wstring computer = T2W(m_clock.Computer);
	TimeOfDayInfoPtr pTimeInfo(FreeTimeOfDayInfo);

	NET_API_STATUS nStatus = ::NetRemoteTOD(computer.c_str(), reinterpret_cast<LPBYTE*>(attachTo(pTimeInfo)));

	// Success?
	if ( (nStatus == NERR_Success) && (!pTimeInfo.empty()) )
	{
		// Get Local and Remote times and compare.
		time_t tLocal  = time(nullptr);
		time_t tRemote = pTimeInfo.get()->tod_elapsedt;
		int    nDiff   = static_cast<int>(tRemote - tLocal);

		m_clock.AbsoluteDiff = abs(nDiff);
		m_clock.RelativeDiff = nDiff;
	}
	// Error.
	else 
	{
		m_clock.AbsoluteDiff = -1;
		m_clock.RelativeDiff = 0;
		m_clock.ErrorCode    = nStatus;
	}
}
