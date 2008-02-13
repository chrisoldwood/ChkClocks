/******************************************************************************
**
** MODULE:		SQLSOURCE.HPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	The CSQLSource class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SQLSOURCE_HPP
#define SQLSOURCE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "FwdDecls.hpp"

/******************************************************************************
** 
** This is the base class for all Database type Data Sources.
**
*******************************************************************************
*/

class CSQLSource
{
public:
	//
	// Constructors/Destructor.
	//
	CSQLSource();
	virtual ~CSQLSource();

	//
	// Connection methods.
	//
	virtual void Open(const tchar* pszConnection) = 0;
	virtual void Close() = 0;

	virtual bool IsOpen() const = 0;

	//
	// Statement methods.
	//
	virtual CSQLParams* CreateParams(const tchar* pszStmt, size_t nParams) = 0;
	virtual void        ExecStmt(const tchar* pszStmt) = 0;
	virtual void        ExecStmt(const tchar* pszStmt, CSQLParams& oParams) = 0;
	virtual CSQLCursor* ExecQuery(const tchar* pszQuery) = 0;

	//
	// Transaction methods.
	//
	virtual bool InTrans() = 0;
	virtual void BeginTrans() = 0;
	virtual void CommitTrans() = 0;
	virtual void RollbackTrans() = 0;

protected:
	//
	// Members.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CSQLSource::CSQLSource()
{
}

inline CSQLSource::~CSQLSource()
{
}

#endif //SQLSOURCE_HPP
