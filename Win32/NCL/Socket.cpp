/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SOCKET.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CSocket class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"
#include <limits.h>

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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

CSocket::CSocket()
	: m_hSocket(INVALID_SOCKET)
{
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

CSocket::~CSocket()
{
	Close();
}

/******************************************************************************
** Method:		Close()
**
** Description:	Close the socket.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSocket::Close()
{
	// Socket still open?
	if (m_hSocket != INVALID_SOCKET)
		closesocket(m_hSocket);

	// Reset members.
	m_hSocket = INVALID_SOCKET;
}

/******************************************************************************
** Method:		Create()
**
** Description:	Create the socket.
**
** Parameters:	See socket().
**
** Returns:		Nothing.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

void CSocket::Create(int nAF, int nType, int nProtocol)
{
	ASSERT(m_hSocket == INVALID_SOCKET);

	// Allocate the socket.
	m_hSocket = socket(nAF, nType, nProtocol);

	if (m_hSocket == INVALID_SOCKET)
		throw CSocketException(CSocketException::E_CREATE_FAILED, CWinSock::LastError());
}

/******************************************************************************
** Method:		Available()
**
** Description:	Queries the number of bytes that can be read without blocking.
**				NB: recv() may return less than this.
**
** Parameters:	None.
**
** Returns:		The number of bytes available.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

int CSocket::Available()
{
	ASSERT(m_hSocket != INVALID_SOCKET);

	fd_set  aoSockets;
	TIMEVAL oWaitTime  = { 0 };
	ulong   lAvailable = 0;

	FD_ZERO(&aoSockets);
	FD_SET(m_hSocket, &aoSockets);

	// Check for readability on the socket.
	if (select(1, &aoSockets, NULL, NULL, &oWaitTime) == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_SELECT_FAILED, CWinSock::LastError());

	// Anything to read?
	if (FD_ISSET(m_hSocket, &aoSockets))
	{
		// How much data is available?
		int nResult = ::ioctlsocket(m_hSocket, FIONREAD, &lAvailable);

		if (nResult == SOCKET_ERROR)
			throw CSocketException(CSocketException::E_PEEK_FAILED, CWinSock::LastError());

		if (lAvailable == 0)
			throw CSocketException(CSocketException::E_DISCONNECTED, WSAESHUTDOWN);
	}

	return lAvailable;
}

/******************************************************************************
** Method:		Send()
**
** Description:	Send the data.
**
** Parameters:	pBuffer		The buffer to send.
**				nBufSize	The buffer size.
**
** Returns:		The number of bytes sent.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

int CSocket::Send(const void* pBuffer, int nBufSize)
{
	ASSERT(m_hSocket != INVALID_SOCKET);

	int nResult = send(m_hSocket, (const char*)pBuffer, nBufSize, 0);

	if (nResult == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_SEND_FAILED, CWinSock::LastError());

	ASSERT(nResult == nBufSize);

	return nResult;
}

/******************************************************************************
** Method:		Recv()
**
** Description:	Read the incoming data.
**
** Parameters:	pBuffer		The buffer to write to.
**				nBufSize	The buffer size.
**
** Returns:		The number of bytes read.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

int CSocket::Recv(void* pBuffer, int nBufSize)
{
	ASSERT(m_hSocket != INVALID_SOCKET);

	int nResult = recv(m_hSocket, (char*)pBuffer, nBufSize, 0);

	if (nResult == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_RECV_FAILED, CWinSock::LastError());

	return nResult;
}

/******************************************************************************
** Method:		Peek()
**
** Description:	Peek at the incoming data.
**
** Parameters:	pBuffer		The buffer to write to.
**				nBufSize	The buffer size.
**
** Returns:		The number of bytes left to read.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

int CSocket::Peek(void* pBuffer, uint nBufSize)
{
	ASSERT(m_hSocket != INVALID_SOCKET);

	int nResult = recv(m_hSocket, (char*)pBuffer, nBufSize, MSG_PEEK);

	if (nResult == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_PEEK_FAILED, CWinSock::LastError());

	return nResult;
}

/******************************************************************************
** Method:		Connect()
**
** Description:	Open a connection.
**
** Parameters:	pszHost		The host name.
**				nPort		The port number.
**
** Returns:		Nothing.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

void CSocket::Connect(const char* pszHost, uint nPort)
{
	ASSERT(m_hSocket == INVALID_SOCKET);
	ASSERT(pszHost   != NULL);
	ASSERT(nPort     <= USHRT_MAX);

	sockaddr_in	addr = { 0 };

	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = inet_addr(pszHost);
	addr.sin_port        = htons((ushort)nPort);

	// Host name isn't an IP Address?
	if (addr.sin_addr.s_addr == INADDR_NONE)
	{
		// Resolve host name.
		hostent* pHost = gethostbyname(pszHost);

		if (pHost == NULL)
			throw CSocketException(CSocketException::E_RESOLVE_FAILED, CWinSock::LastError());

		memcpy(&addr.sin_addr, pHost->h_addr_list[0], pHost->h_length);
	}

	// Create the socket.
	Create(AF_INET, Type(), Protocol());

	// Connect to host.
	if (connect(m_hSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_CONNECT_FAILED, CWinSock::LastError());
}


/******************************************************************************
** Method:		IsAddress()
**
** Description:	Queries if the host name is a raw IP address.
**
** Parameters:	pszHost		The host name.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CSocket::IsAddress(const char* pszHost)
{
	return (inet_addr(pszHost) != INADDR_NONE);
}

/******************************************************************************
** Method:		Resolve()
**
** Description:	Resolves the host name into an IP address.
**
** Parameters:	pszHost		The host name.
**
** Returns:		The IP address.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

in_addr CSocket::Resolve(const char* pszHost)
{
	in_addr addr;

	// Is already an IP address?
	addr.s_addr = inet_addr(pszHost);

	if (addr.s_addr == INADDR_NONE)
	{
		// Resolve host name.
		hostent* pHost = gethostbyname(pszHost);

		if (pHost == NULL)
			throw CSocketException(CSocketException::E_RESOLVE_FAILED, CWinSock::LastError());

		memcpy(&addr, pHost->h_addr_list[0], pHost->h_length);
	}

	return addr;
}

/******************************************************************************
** Method:		ResolveStr()
**
** Description:	Resolves the host name into an IP address string.
**
** Parameters:	pszHost		The host name.
**
** Returns:		The IP address as a string.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

CString CSocket::ResolveStr(const char* pszHost)
{
	// Is already an IP address?
	if (IsAddress(pszHost))
		return pszHost;

	in_addr addr;

	// Resolve host name.
	hostent* pHost = gethostbyname(pszHost);

	if (pHost == NULL)
		throw CSocketException(CSocketException::E_RESOLVE_FAILED, CWinSock::LastError());

	memcpy(&addr, pHost->h_addr_list[0], pHost->h_length);

	return inet_ntoa(addr);
}
