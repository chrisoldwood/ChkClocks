/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PATH.HPP
** COMPONENT:	Windows C++ Library.
** DESCRIPTION:	The CPath class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PATH_HPP
#define PATH_HPP

// Forward declarations.
class CWnd;

/******************************************************************************
** 
** A path is a string derived class that is used to manipulate filenames and
** directory paths.
**
*******************************************************************************
*/

class CPath : public CString
{
public:
	//
	// Constructors/Destructor.
	//
	CPath();
    CPath(const char*    pszPath);
	CPath(const CString& strSrc);
	CPath(const CPath&   strSrc);

	//
	// File/Dir attributes.
	//
    bool Exists() const;
    bool ReadOnly() const;

	// Max length of path.
	enum { MAX_LENGTH = 260 };

	//
	// Path components.
	//
	CPath   Directory() const;
	CString FileName() const;
	CString FileTitle() const;
	CString FileExt() const;

	//
	// Operations.
	// TODO: Move to CFile.
/*	//
	bool RenameFile(const char* pszPath);
	bool DeleteFile();
*/
	//
	// File Open dialog.
	enum DlgMode
	{
		OpenFile,
		SaveFile,
		SelectFile
	};
    
	bool Select(const CWnd& rParent, DlgMode eMode, const char* pszExts,
				const char* pszDefExt, const char* pszDir = NULL);
	bool SelectDir(const CWnd& rParent, const char* pszTitle);
	bool SelectComputer(const CWnd& rParent, const char* pszTitle);

	//
	// Common path root directories.
	//
	static CPath CurrWorkDir();
	static CPath AppDir();
	static CPath WindowsDir();
	static CPath WindowsSystemDir();
	static CPath TempDir();

	//
	// Operators.
	//
	const CPath& operator=(const CString& rSrc);
	const CPath& operator=(const char* pszBuffer);
	
	void operator+=(const char* pszPath);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CPath::CPath(const char* pszPath)
{
	Copy(pszPath);
}

inline CPath::CPath(const CString& strSrc)
{
	Copy(strSrc);
}

inline CPath::CPath(const CPath& strSrc)
{
	Copy(strSrc);
}

inline const CPath& CPath::operator=(const CString& rSrc)
{
	Copy(rSrc);
	return *this;
}

inline const CPath& CPath::operator=(const char* pszBuffer)
{
	Copy(pszBuffer);
	return *this;
}

inline void CPath::operator+=(const char* pszPath)
{
	ASSERT(pszPath);
	CString::operator +=("\\");
	CString::operator +=(pszPath);
}

/******************************************************************************
** 
** Global string operators.
**
*******************************************************************************
*/

inline CPath operator+(const CPath& strLHS, const CPath& strRHS)
{
	CPath str;

	str  = strLHS;
	str += strRHS;

	return str;
}

inline CPath operator+(const CPath& strLHS, const CString& strRHS)
{
	CPath str;

	str  = strLHS;
	str += strRHS;

	return str;
}

inline CPath operator+(const CPath& strLHS, const char* pszRHS)
{
	CPath str;

	str  = strLHS;
	str += pszRHS;

	return str;
}

#endif //PATH_HPP
