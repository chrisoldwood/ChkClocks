/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		STRCVTEXCEPTION.HPP
** COMPONENT:	Windows C++ Library
** DESCRIPTION:	The CStrCvtException class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef STRCVTEXCEPTION_HPP
#define STRCVTEXCEPTION_HPP

/******************************************************************************
** 
** The exception class thrown by the CStrCvt functions.
**
*******************************************************************************
*/

class CStrCvtException : public CException
{
public:
	//
	// Exception codes.
	//
	enum ErrCode
	{
		E_INVALID_FORMAT = 10,		// String has an invalid format.
		E_INVALID_RANGE  = 11,		// String value too small/large for type.
	};

	//
	// Constructors/Destructor.
	//
	CStrCvtException(int eErrCode);
	~CStrCvtException();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // STRCVTEXCEPTION_HPP
