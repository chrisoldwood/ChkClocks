/******************************************************************************
**
** MODULE:		SQLEXCEPTION.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CSQLException class definition.
**
*******************************************************************************
*/

#include "MDBL.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	eErrCode	The exception code.
**				strSQLStmt	The statement\connection being executed.
**				strSQLErr	The error returned by the source.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CSQLException::CSQLException(int eErrCode, const CString& strSQLStmt, const CString& strSQLErr)
	: m_eError(eErrCode)
{
	// Convert error to string.
	switch(eErrCode)
	{
		case E_CONNECT_FAILED:
			m_strError  = "Failed to connect to Database:\n\n";
			break;

		case E_EXEC_FAILED:
			m_strError  = "Failed to execute the statement:\n\n";
			break;

		case E_FETCH_FAILED:
			m_strError  = "Failed to fetch result set:\n\n";
			break;

		// Shouldn't happen!
		default:
			ASSERT(false);
			break;
	}

	// Append statement and reason.
	m_strError += strSQLStmt;
	m_strError += "\n\nReason:\n\n";
	m_strError += strSQLErr;
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

CSQLException::~CSQLException()
{
}
