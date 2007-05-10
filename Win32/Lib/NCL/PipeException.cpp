/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PIPEEXCEPTION.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CPipeException class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

/******************************************************************************
** Method:		Constructor.
**
** Description:	Formats an exception which does not use GetLastError().
**
** Parameters:	eErrCode	The exception code
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CPipeException::CPipeException(int eErrCode)
	: CException(eErrCode)
	, m_hResult(NO_ERROR)
{
	// Convert error to string.
	switch(eErrCode)
	{
		case E_BAD_PROTOCOL:
			m_strErrorText.Format("Invalid pipe protocol version");
			break;

		// Shouldn't happen!
		default:
			ASSERT_FALSE();
			break;
	}
}

/******************************************************************************
** Method:		Constructor.
**
** Description:	Formats an exception which includes GetLastError().
**
** Parameters:	eErrCode	The exception code
**				hResult		The result of GetLastError().
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CPipeException::CPipeException(int eErrCode, HRESULT hResult)
	: CException(eErrCode)
	, m_hResult(hResult)
{
	// Convert error to string.
	switch(eErrCode)
	{
		case E_CREATE_FAILED:
			m_strErrorText.Format("Failed to create pipe:\n\n%s", CApp::This().FormatError(hResult));
			break;

		case E_OPEN_FAILED:
			m_strErrorText.Format("Failed to open pipe:\n\n%s", CApp::This().FormatError(hResult));
			break;

		case E_ACCEPT_FAILED:
			m_strErrorText.Format("Failed to accept pipe connection:\n\n%s", CApp::This().FormatError(hResult));
			break;

		case E_PEEK_FAILED:
			m_strErrorText.Format("Failed to peek from pipe:\n\n%s", CApp::This().FormatError(hResult));
			break;

		case E_READ_FAILED:
			m_strErrorText.Format("Failed to read from pipe:\n\n%s", CApp::This().FormatError(hResult));
			break;

		case E_WRITE_FAILED:
			m_strErrorText.Format("Failed to write to pipe:\n\n%s", CApp::This().FormatError(hResult));
			break;

		// Shouldn't happen!
		default:
			ASSERT_FALSE();
			break;
	}
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

CPipeException::~CPipeException()
{
}