////////////////////////////////////////////////////////////////////////////////
//! \file   ParseException.hpp
//! \brief  The ParseException class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_PARSEEXCEPTION_HPP
#define CORE_PARSEEXCEPTION_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The type of exception thrown for parsing errors.

class ParseException : public Core::Exception
{
public:
	//! Default constructor.
	ParseException(const tstring& strDetails);

	//! Destructor.
	virtual ~ParseException();
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline ParseException::ParseException(const tstring& strDetails)
	: Exception(strDetails)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline ParseException::~ParseException()
{
}

//namespace Core
}

#endif // CORE_PARSEEXCEPTION_HPP
