/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		WINSOCK.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CWinSock class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "WinSock.hpp"
#include <WCL/Module.hpp>
#include "Socket.hpp"
#include "SocketException.hpp"

// Linker directive.
#pragma comment(lib, "ws2_32")

/******************************************************************************
**
** Class members.
**
*******************************************************************************
*/

bool    CWinSock::g_bStarted = false;
WSADATA CWinSock::g_oWSAData = { 0 };
uint    CWinSock::g_nSockMsg = 0;
HWND    CWinSock::g_hSockWnd = NULL;
CWinSock::SocketMap* CWinSock::g_pSockMap = NULL;

/******************************************************************************
** Method:		Startup()
**
** Description:	Iniitalise the WinSock library.
**
** Parameters:	nMajorVer, nMinorVer	The expected version number.
**
** Returns:		See WSAStartup().
**
*******************************************************************************
*/

int CWinSock::Startup(uint nMajorVer, uint nMinorVer)
{
	g_bStarted = true;

	// Get an ID for the socket messages.
	g_nSockMsg = ::RegisterWindowMessage("WM_WSAASYNCSELECT");

	ASSERT(g_nSockMsg != 0);

	WNDCLASS oWndClass = { 0 };

	oWndClass.lpfnWndProc   = WindowProc;
	oWndClass.hInstance     = CModule::This().Handle();
	oWndClass.lpszClassName = "WinSockClass";

	// Register the socket window class.
	::RegisterClass(&oWndClass);

	// Create the socket window.
	g_hSockWnd = ::CreateWindowEx(0, "WinSockClass", "WinSock Message Window",
								WS_OVERLAPPED, 0, 0, 0, 0, HWND_DESKTOP, NULL,
								CModule::This().Handle(), NULL);

	ASSERT(g_hSockWnd != NULL);

	// Create the socket handle map.
	g_pSockMap = new SocketMap;

	return ::WSAStartup(MAKEWORD(nMajorVer, nMinorVer), &g_oWSAData);
}

/******************************************************************************
** Method:		Cleanup()
**
** Description:	Shutdown the WinSock library.
**
** Parameters:	None.
**
** Returns:		See WSACleanup().
**
*******************************************************************************
*/

int CWinSock::Cleanup()
{
	ASSERT((g_pSockMap == nullptr) || (g_pSockMap->empty()));

	// Destroy the socket handle map.
	delete g_pSockMap;

	// Destroy the socket window.
	if (g_hSockWnd != NULL)
		::DestroyWindow(g_hSockWnd);

	return ::WSACleanup();
}

/******************************************************************************
** Method:		ErrorToSymbol()
**
** Description:	Convert the error code to its C++ #defined symbol.
**
** Parameters:	nError		The error.
**
** Returns:		The symbol or the error number, if unrecognised.
**
*******************************************************************************
*/

CString CWinSock::ErrorToSymbol(int nError)
{
	// Known value?
	switch (nError)
	{
		case WSABASEERR:			return "WSABASEERR";
		case WSAEINTR:				return "WSAEINTR";
		case WSAEBADF:				return "WSAEBADF";
		case WSAEACCES:				return "WSAEACCES";
		case WSAEFAULT:				return "WSAEFAULT";
		case WSAEINVAL:				return "WSAEINVAL";
		case WSAEMFILE:				return "WSAEMFILE";
		case WSAEWOULDBLOCK:		return "WSAEWOULDBLOCK";
		case WSAEINPROGRESS:		return "WSAEINPROGRESS";
		case WSAEALREADY:			return "WSAEALREADY";
		case WSAENOTSOCK:			return "WSAENOTSOCK";
		case WSAEDESTADDRREQ:		return "WSAEDESTADDRREQ";
		case WSAEMSGSIZE:			return "WSAEMSGSIZE";
		case WSAEPROTOTYPE:			return "WSAEPROTOTYPE";
		case WSAENOPROTOOPT:		return "WSAENOPROTOOPT";
		case WSAEPROTONOSUPPORT:	return "WSAEPROTONOSUPPORT";
		case WSAESOCKTNOSUPPORT:	return "WSAESOCKTNOSUPPORT";
		case WSAEOPNOTSUPP:			return "WSAEOPNOTSUPP";
		case WSAEPFNOSUPPORT:		return "WSAEPFNOSUPPORT";
		case WSAEAFNOSUPPORT:		return "WSAEAFNOSUPPORT";
		case WSAEADDRINUSE:			return "WSAEADDRINUSE";
		case WSAEADDRNOTAVAIL:		return "WSAEADDRNOTAVAIL";
		case WSAENETDOWN:			return "WSAENETDOWN";
		case WSAENETUNREACH:		return "WSAENETUNREACH";
		case WSAENETRESET:			return "WSAENETRESET";
		case WSAECONNABORTED:		return "WSAECONNABORTED";
		case WSAECONNRESET:			return "WSAECONNRESET";
		case WSAENOBUFS:			return "WSAENOBUFS";
		case WSAEISCONN:			return "WSAEISCONN";
		case WSAENOTCONN:			return "WSAENOTCONN";
		case WSAESHUTDOWN:			return "WSAESHUTDOWN";
		case WSAETOOMANYREFS:		return "WSAETOOMANYREFS";
		case WSAETIMEDOUT:			return "WSAETIMEDOUT";
		case WSAECONNREFUSED:		return "WSAECONNREFUSED";
		case WSAELOOP:				return "WSAELOOP";
		case WSAENAMETOOLONG:		return "WSAENAMETOOLONG";
		case WSAEHOSTDOWN:			return "WSAEHOSTDOWN";
		case WSAEHOSTUNREACH:		return "WSAEHOSTUNREACH";
		case WSAENOTEMPTY:			return "WSAENOTEMPTY";
		case WSAEPROCLIM:			return "WSAEPROCLIM";
		case WSAEUSERS:				return "WSAEUSERS";
		case WSAEDQUOT:				return "WSAEDQUOT";
		case WSAESTALE:				return "WSAESTALE";
		case WSAEREMOTE:			return "WSAEREMOTE";
		case WSAEDISCON:			return "WSAEDISCON";
		case WSASYSNOTREADY:		return "WSASYSNOTREADY";
		case WSAVERNOTSUPPORTED:	return "WSAVERNOTSUPPORTED";
		case WSANOTINITIALISED:		return "WSANOTINITIALISED";
		case WSAHOST_NOT_FOUND:		return "WSAHOST_NOT_FOUND";
		case WSATRY_AGAIN:			return "WSATRY_AGAIN";
		case WSANO_RECOVERY:		return "WSANO_RECOVERY";
		case WSANO_DATA:			return "WSANO_DATA";
	}

	char szError[50] = "#";

	// Unrecognised, format as a number.
	_itoa(nError, szError+1, 10);

	return szError;
}

/******************************************************************************
** Method:		WindowProc()
**
** Description:	The socket window callback function.
**
** Parameters:	See WindowProc().
**
** Returns:		See WindowProc().
**
*******************************************************************************
*/

LRESULT CALLBACK CWinSock::WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// Is a socket message?
	if (nMsg == g_nSockMsg)
	{
		ASSERT(g_pSockMap != NULL);

		// Extract message details.
		SOCKET hSocket = wParam;
		int    nEvent  = WSAGETSELECTEVENT(lParam);
		int    nError  = WSAGETSELECTERROR(lParam);

		try
		{
//			TRACE3("Socket message: 0x%08X %u %u\n", hSocket, nEvent, nError);

			SocketMap::const_iterator it = g_pSockMap->find(hSocket);

			// Socket mapped?
			if (it != g_pSockMap->end())
			{
				CSocket* pSocket = it->second;

				ASSERT(pSocket != NULL);

				// Forward event.
				pSocket->OnAsyncSelect(nEvent, nError);

			}

			return 0;
		}
		catch (const std::exception& e)
		{
			WCL::ReportUnhandledException("Unexpected exception caught in CWinSock::WindowProc()\n\n"
											"Message: Event=0x%08X, Error=0x%08X\n\n%s",
											nEvent, nError, e.what());
		}
		catch (...)
		{
			WCL::ReportUnhandledException("Unexpected unknown exception caught in DlgProc()\n\n"
											"Message: Event=0x%08X, Error=0x%08X",
											nEvent, nError);
		}
	}

	// Do default processing.
	return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
}

/******************************************************************************
** Method:		BeginAsyncSelect()
**
** Description:	Switch a socket to non-blocking mode and add it to the socket
**				handle map.
**
** Parameters:	pSocket		The socket.
**				lEventMask	The events to notify.
**
** Returns:		Nothing.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

void CWinSock::BeginAsyncSelect(CSocket* pSocket, long lEventMask)
{
	ASSERT(pSocket    != NULL);
	ASSERT(g_pSockMap != NULL);

	// Get the socket handle.
	SOCKET hSocket = pSocket->Handle();

	ASSERT(hSocket != INVALID_SOCKET);

	// Add handle<->socket mapping.
	g_pSockMap->insert(std::make_pair(hSocket, pSocket));

	// Start events...
	if (::WSAAsyncSelect(hSocket, g_hSockWnd, g_nSockMsg, lEventMask) == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_SELECT_FAILED, CWinSock::LastError());
}

/******************************************************************************
** Method:		EndAsyncSelect()
**
** Description:	The socket window callback function.
**
** Parameters:	pSocket		The socket.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CWinSock::EndAsyncSelect(CSocket* pSocket)
{
	ASSERT(pSocket != NULL);

	// Get the socket handle.
	SOCKET hSocket = pSocket->Handle();

	ASSERT(hSocket != INVALID_SOCKET);

	// Stop events.
	::WSAAsyncSelect(hSocket, g_hSockWnd, g_nSockMsg, 0);

	// Remove handle<->socket mapping.
	SocketMap::iterator it = g_pSockMap->find(hSocket);

	if (it != g_pSockMap->end())
		g_pSockMap->erase(it);
}

/******************************************************************************
** Method:		ProcessSocketMsgs()
**
** Description:	Process any socket messages in the message queue.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CWinSock::ProcessSocketMsgs()
{
	MSG oMsg;

	while (::PeekMessage(&oMsg, g_hSockWnd, g_nSockMsg, g_nSockMsg, PM_REMOVE))
		::DispatchMessage(&oMsg);
}
