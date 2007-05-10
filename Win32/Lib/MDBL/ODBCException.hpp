/******************************************************************************
**
** MODULE:		ODBCEXCEPTION.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CODBCException class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ODBCEXCEPTION_HPP
#define ODBCEXCEPTION_HPP

#include <sql.h>
#include <sqlext.h>

#include "SQLException.hpp"

/******************************************************************************
** 
** This is the exception class thrown by ODBC data source objects.
**
*******************************************************************************
*/

class CODBCException : public CSQLException  
{
public:
	//
	// Constructors/Destructor.
	//
	CODBCException(int eErrCode, const char* pszSQLStmt, SQLHANDLE hHandle, SQLSMALLINT nType);
	virtual ~CODBCException();

protected:
	// Max size of error buffer.
	enum { MAX_ERR_BUF_LEN = 256 };

	//
	// Internal methods.
	//
	CString LastError(SQLHANDLE hHandle, SQLSMALLINT nType) const;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // ODBCEXCEPTION_HPP