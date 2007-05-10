/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		LABEL.HPP
** COMPONENT:	Windows C++ Library.
** DESCRIPTION:	The CLabel class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef LABEL_HPP
#define LABEL_HPP

/******************************************************************************
** 
** This is a text label.
**
*******************************************************************************
*/

class CLabel : public CStdWnd
{
public:
	//
	// Constructors/Destructor.
	//
	CLabel();

	//
	// Standard methods.
	//
	void Text(const char* pszText);

protected:
	//
	// Members.
	//
	
	//
	// Window creation template methods.
	//
	virtual void GetCreateParams(WNDCREATE& rParams);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline void CLabel::Text(const char* pszText)
{
	Title(pszText);
}

#endif //LABEL_HPP