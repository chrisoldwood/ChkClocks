/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CHECKJOB.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CCheckJob class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include <lm.h>

#ifndef _DEBUG
#define	ATLASSERT
#endif

// conditional expression is constant
#pragma warning ( disable : 4127 )

#include <atlconv.h>

// conditional expression is constant
#pragma warning ( default : 4127 )

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
	USES_CONVERSION;

	// Get the remote time.
	const char*         paszComputer = m_oRow[CClocks::COMPUTER];
	const wchar_t*      pwszComputer = A2W(paszComputer);
	LPTIME_OF_DAY_INFO	pTimeInfo    = NULL;
	NET_API_STATUS		nStatus      = NERR_Success;

	nStatus = ::NetRemoteTOD(pwszComputer, (LPBYTE*)&pTimeInfo);

	// Success?
	if ( (nStatus == NERR_Success) && (pTimeInfo != NULL) )
	{
		// Get Local and Remote times and compare.
		time_t tLocal  = time(NULL);
		time_t tRemote = pTimeInfo->tod_elapsedt;
		int    nDiff   = tRemote - tLocal;

		m_oRow[CClocks::ABS_DIFF] = abs(nDiff);
		m_oRow[CClocks::REL_DIFF] = nDiff;
	}
	// Error.
	else 
	{
		m_oRow[CClocks::ABS_DIFF]   = -1;
		m_oRow[CClocks::REL_DIFF]   = 0;
		m_oRow[CClocks::ERROR_CODE] = (int)nStatus;
	}

	// Cleanup.
	if (pTimeInfo != NULL)
		::NetApiBufferFree(pTimeInfo);
}
