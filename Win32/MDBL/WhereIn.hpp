/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		WHEREIN.HPP
** COMPONENT:	Memory Database Library
** DESCRIPTION:	The CWhereIn class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef WHEREIN_HPP
#define WHEREIN_HPP

/******************************************************************************
** 
** This class is used for WHERE clause comparisons of multiple value.
**
*******************************************************************************
*/

class CWhereIn : public CWhere
{
public:
	//
	// Constructors/Destructor.
	//
	CWhereIn(int nColumn, const CValueSet& oValueSet);
	CWhereIn(const CWhereIn& oSrc);
	virtual ~CWhereIn();
	
	//
	// Methods
	//
	virtual bool Matches(const CRow& oRow) const;

	virtual CWhere* Clone() const;

private:
	//
	// Members.
	//
	int			m_nColumn;		// The column to check.
	CValueSet	m_oValueSet;	// The values to match.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // WHEREIN_HPP