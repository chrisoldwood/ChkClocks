/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MAP.HPP
** COMPONENT:	Windows C++ Library.
** DESCRIPTION:	The CMap class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MAP_HPP
#define MAP_HPP

/******************************************************************************
** 
** This is the base class used for items stored in a map collection.
**
*******************************************************************************
*/

class CMapItem
{
public:
	//
	// Constructors/Destructor.
	//
	CMapItem();
	virtual ~CMapItem();

	//
	// Methods.
	//
	virtual uint Key() const = 0;
	virtual bool operator==(const CMapItem& rRHS) const = 0;
	virtual bool operator!=(const CMapItem& rRHS) const;

	//
	// Members.
	//
	CMapItem*	m_pNext;	// The next item in the collision chain.
};

/******************************************************************************
** 
** The is the base class for all map collections.
**
*******************************************************************************
*/

class CMap
{
public:
	//
	// Methods.
	//
	int  Count() const;
	void RemoveAll();

	void Reserve(int nItems);

protected:
	//
	// Constructors/Destructor.
	//
	CMap();
	~CMap();
	
	//
	// External methods.
	//
	void      Add(CMapItem& rItem);
	void      Remove(const CMapItem& rItem);
	CMapItem* Find(const CMapItem& rItem) const;

	//
	// Internal methods.
	//
	virtual int Hash(const CMapItem& rItem) const;

	//
	// Members.
	//
	int			m_iSize;		// The size of the map.
	CMapItem**	m_pMap;			// The array of map buckets.
	int			m_iCount;		// The number of items in the map.

	// Map size table size.
	enum { NUM_MAP_SIZES = 15};

	// Array of map sizes.
	static int s_aiSizes[NUM_MAP_SIZES];

	// Max expected chain length.
	enum { MAX_CHAIN_LEN = 4 };

	// Friends.
	friend class CMapIter;

private:
	// Disallow copying and assignment.
	CMap(const CMap&);
	void operator=(const CMap&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CMap::Count() const
{
	return m_iCount;
}

inline bool CMapItem::operator!=(const CMapItem& rRHS) const
{
	return !(*this == rRHS);
}

#endif //MAP_HPP
