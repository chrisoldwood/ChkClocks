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

CCheckJob::CCheckJob(CRow& oRow)
	: m_oRow(oRow)
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
	// Get the remote time.
	const tchar*        pszComputer  = m_oRow[CClocks::COMPUTER];
	LPTIME_OF_DAY_INFO	pTimeInfo    = NULL;
	NET_API_STATUS		nStatus      = NERR_Success;

	nStatus = ::NetRemoteTOD(T2W(pszComputer), reinterpret_cast<LPBYTE*>(&pTimeInfo));

	// Success?
	if ( (nStatus == NERR_Success) && (pTimeInfo != NULL) )
	{
		// Get Local and Remote times and compare.
		time_t tLocal  = time(NULL);
		time_t tRemote = pTimeInfo->tod_elapsedt;
		int    nDiff   = static_cast<int>(tRemote - tLocal);

		m_oRow[CClocks::ABS_DIFF] = abs(nDiff);
		m_oRow[CClocks::REL_DIFF] = nDiff;
	}
	// Error.
	else 
	{
		m_oRow[CClocks::ABS_DIFF]   = -1;
		m_oRow[CClocks::REL_DIFF]   = 0;
		m_oRow[CClocks::ERROR_CODE] = static_cast<int>(nStatus);
	}

	// Cleanup.
	if (pTimeInfo != NULL)
		::NetApiBufferFree(pTimeInfo);
}
