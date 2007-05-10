/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLIPBOARD.HPP
** COMPONENT:	Windows C++ Library.
** DESCRIPTION:	The CClipboard class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CLIPBOARD_HPP
#define CLIPBOARD_HPP

/* WINVER >= 0x0500 */
#ifndef CF_DIBV5
#define CF_DIBV5	17
#endif 

/******************************************************************************
** 
** This is a CStream derived class used to access the clipboard.
**
*******************************************************************************
*/

class CClipboard : public CStream
{
public:
	//
	// Constructors/Destructor.
	//
	CClipboard();
	~CClipboard();
	
	//
	// Properties.
	//
	ulong Size() const;

	//
	// Open/Close operations.
	//
	void Open(uint nMode, uint iFormat);
	void Close();

	//
	// Overriden generic operations.
	//
	virtual void  Read(void* pBuffer, uint iNumBytes);
	virtual void  Write(const void* pBuffer, uint iNumBytes);
	virtual ulong Seek(ulong lPos, uint nFrom = FILE_BEGIN);
	virtual bool  IsEOF();
	virtual void  Throw(int eErrCode);

	//
	// Helper methods.
	//
	static bool CopyText(HWND hOwner, const char* pszText);

	static bool IsEmpty();
	static bool IsFormatAvail(uint nFormat);
	static bool PasteText(CString& strString);

	//
	// Format methods.
	//
	static bool    IsStdFormat(uint nFormat);
	static uint    RegisterFormat(const char* pszFormat);
	static CString FormatName(uint nFormat);
	static uint    FormatHandle(const char* pszFormat);

protected:
	//
	// Members.
	//
	CBuffer*	m_pBuffer;		// Buffer for stream.
	CMemStream*	m_pStream;		// Used to implement stream methods.
	uint		m_iFormat;		// The format of the clipboard data.

private:
	/**************************************************************************
	** Structure used for standard formats table
	*/

	struct FmtEntry
	{
		uint		m_nFormat;		// Handle.
		const char*	m_pszFormat;	// Name.
	};

	static FmtEntry s_oStdFormats[];
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline ulong CClipboard::Size() const
{
	ASSERT(m_pStream != NULL);

	return m_pStream->Size();
}

inline void CClipboard::Read(void* pBuffer, uint iNumBytes)
{
	ASSERT(m_pStream != NULL);

	m_pStream->Read(pBuffer, iNumBytes);
}

inline void CClipboard::Write(const void* pBuffer, uint iNumBytes)
{
	ASSERT(m_pStream != NULL);

	m_pStream->Write(pBuffer, iNumBytes);
}

inline ulong CClipboard::Seek(ulong lPos, uint nFrom)
{
	ASSERT(m_pStream != NULL);

	return m_pStream->Seek(lPos, nFrom);
}

inline bool CClipboard::IsEOF()
{
	ASSERT(m_pStream != NULL);

	return m_pStream->IsEOF();
}

inline void CClipboard::Throw(int eErrCode)
{
	ASSERT(m_pStream != NULL);

	m_pStream->Throw(eErrCode);
}

#endif //CLIPBOARD_HPP