/******************************************************************************
**
** MODULE:		COLUMN.CPP
** COMPONENT:	Memory Database Library.
** DESCRIPTION:	CColumn class definition.
**
*******************************************************************************
*/

#include "MDBL.hpp"

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

CColumn::CColumn(CTable& oTable, const char* pszName, COLTYPE eType, int nLength, int nAllocSize, int nFlags)
	: m_oTable(oTable)
	, m_strName(pszName)
	, m_eColType(eType)
	, m_eStgType(ColToStgType(eType))
	, m_nLength(nLength)
	, m_nAllocSize((nAllocSize + 3) & ~3)
	, m_nFlags(nFlags)
	, m_pFKTable(NULL)
	, m_nFKColumn(-1)
	, m_pIndex(NULL)
{
	ASSERT(pszName      != NULL);
	ASSERT(m_eStgType   != MDST_NULL);
	ASSERT(!(Nullable() && Unique()));
	ASSERT(!(Nullable() && PrimaryKey()));
	ASSERT(!ForeignKey());
	ASSERT(!((m_eStgType == MDST_POINTER) && (!Transient())));
}

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

CColumn::CColumn(CTable& oTable, const char* pszName, CTable& oFKTable, int nFKColumn, const CColumn& oFKColumn, int nFlags)
	: m_oTable(oTable)
	, m_strName(pszName)
	, m_eColType(oFKColumn.ColType())
	, m_eStgType(ColToStgType(oFKColumn.ColType()))
	, m_nLength(oFKColumn.Length())
	, m_nAllocSize(oFKColumn.AllocSize())
	, m_nFlags(nFlags)
	, m_pFKTable(&oFKTable)
	, m_nFKColumn(nFKColumn)
	, m_pIndex(NULL)
{
	ASSERT(pszName      != NULL);
	ASSERT(m_eStgType   != MDST_NULL);
	ASSERT(m_nAllocSize >  0);
	ASSERT(!(Nullable() && Unique()));
	ASSERT(ForeignKey());
	ASSERT(m_eStgType != MDST_POINTER);

	// If Identity, convert to int.
	if (m_eColType == MDCT_IDENTITY)
		m_eColType = MDCT_INT;
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

CColumn::~CColumn()
{
	delete m_pIndex;
}

/******************************************************************************
** Method:		ColToStgType()
**
** Description:	Finds the storage type required for the given column type.
**
** Parameters:	None.
**
** Returns:		The storage type.
**
*******************************************************************************
*/

STGTYPE CColumn::ColToStgType(COLTYPE eType)
{
	switch (eType)
	{
		case MDCT_INT:			return MDST_INT;
		case MDCT_DOUBLE:		return MDST_DOUBLE;
		case MDCT_CHAR:			return MDST_CHAR;
		case MDCT_FXDSTR:		return MDST_STRING;
		case MDCT_VARSTR:		return MDST_STRING;
		case MDCT_BOOL:			return MDST_BOOL;
		case MDCT_IDENTITY:		return MDST_INT;
		case MDCT_DATETIME:		return MDST_TIME_T;
		case MDCT_DATE:			return MDST_TIME_T;
		case MDCT_TIME:			return MDST_TIME_T;
		case MDCT_TIMESTAMP:	return MDST_TIMESTAMP;
		case MDCT_VOIDPTR:		return MDST_POINTER;
		case MDCT_ROWPTR:		return MDST_POINTER;
		case MDCT_ROWSETPTR:	return MDST_POINTER;
		default:				ASSERT(false);	break;
	}

	return MDST_NULL;
}

/******************************************************************************
** Method:		Index()
**
** Description:	Sets a new index on the column, and deletes the old one.
**
** Parameters:	pIndex	The index to use.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CColumn::Index(CIndex* pIndex)
{
	delete m_pIndex;
	m_pIndex = pIndex;
}

/******************************************************************************
** Method:		DisplayWidth()
**
** Description:	Gets the char width of the column for display.
**
** Parameters:	bDebug		Get width for debug format strings?
**
** Returns:		The width.
**
*******************************************************************************
*/

int CColumn::DisplayWidth(bool bDebug) const
{
	// Normal format?
	if (!bDebug)
	{
		switch (m_eColType)
		{
			case MDCT_INT:			return 10;
			case MDCT_DOUBLE:		return 15;
			case MDCT_CHAR:			return 1;
			case MDCT_FXDSTR:		return m_nLength;
			case MDCT_VARSTR:		return m_nLength;
			case MDCT_BOOL:			return 1;
			case MDCT_IDENTITY:		return 10;
			case MDCT_DATETIME:		return 17;
			case MDCT_DATE:			return 8;
			case MDCT_TIME:			return 8;
			case MDCT_TIMESTAMP:	return 17;
			case MDCT_VOIDPTR:		return 8;
			case MDCT_ROWPTR:		return 8;
			case MDCT_ROWSETPTR:	return 8;
			default:				ASSERT(false);	break;
		}
	}
	// Debug format?
	else
	{
		int nMin = (Nullable()) ? 5 : 0;

		switch (m_eStgType)
		{
			case MDST_INT:			return max(nMin, 10);
			case MDST_DOUBLE:		return max(nMin, 15);
			case MDST_CHAR:			return max(nMin, 1);
			case MDST_STRING:		return max(nMin, m_nLength);
			case MDST_BOOL:			return max(nMin, 1);
			case MDST_TIME_T:		return max(nMin, 17);
			case MDST_TIMESTAMP:	return max(nMin, 17);
			case MDST_POINTER:		return max(nMin, 10);
			default:				ASSERT(false); break;
		}
	}

	return -1;
}
