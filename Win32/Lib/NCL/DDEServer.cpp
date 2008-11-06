/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDESERVER.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDEServer class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "DDEServer.hpp"
#include <WCL/StrArray.hpp>
#include "DDEException.hpp"
#include "DDESvrConv.hpp"
#include "DDEString.hpp"
#include "IDDEServerListener.hpp"
#include "DDEData.hpp"

/******************************************************************************
**
** Class members.
**
*******************************************************************************
*/

// The single DDE Client object.
CDDEServer* CDDEServer::g_pDDEServer = NULL;

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

CDDEServer::CDDEServer(DWORD dwFlags)
{
	m_eType = SERVER;

	Initialise(dwFlags);

	ASSERT(g_pDDEServer == NULL);
	g_pDDEServer = this;
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

CDDEServer::~CDDEServer()
{
	ASSERT(g_pDDEServer == this);
	g_pDDEServer = NULL;

	Uninitialise();

	ASSERT(m_aoConvs.Size() == 0);
	ASSERT(m_aoListeners.Size() == 0);
}

/******************************************************************************
** Method:		Initialise()
**
** Description:	Initialise the DDE Server with the DDEML libray.
**
** Parameters:	dwFlags		The additional DDEML initialisation flags.
**
** Returns:		Nothing.
**
** Exceptions:	CDDEException.
**
*******************************************************************************
*/

void CDDEServer::Initialise(DWORD dwFlags)
{
	// Register with DDEML.
	UINT nResult = ::DdeInitialize(&m_dwInst, DDECallbackProc, APPCLASS_STANDARD | dwFlags, 0);

	// Failed?
	if (nResult != DMLERR_NO_ERROR)
		throw CDDEException(CDDEException::E_INIT_FAILED, ::DdeGetLastError(m_dwInst));
}

/******************************************************************************
** Method:		Uninitialise()
**
** Description:	Uninitialise the DDE Client with the DDEML libray.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::Uninitialise()
{
	// Cleanup.
	if (m_dwInst != 0)
		::DdeUninitialize(m_dwInst);

	// Delete all conversations.
	for (size_t i = 0; i < m_aoConvs.Size(); ++i)
		delete m_aoConvs[i];

	// Reset members.
	m_dwInst = 0;
	m_aoConvs.RemoveAll();
	m_aoListeners.RemoveAll();
}

/******************************************************************************
** Method:		Register()
**
** Description:	Register the service name.
**
** Parameters:	pszService	The service name.
**
** Returns:		Nothing.
**
** Exceptions:	CDDEException.
**
*******************************************************************************
*/

void CDDEServer::Register(const tchar* pszService)
{
	ASSERT(pszService != NULL);

	CDDEString strService(this, pszService);

	// Try and register the service name.
	HDDEDATA hResult = ::DdeNameService(m_dwInst, strService, NULL, DNS_REGISTER);

	if (hResult == NULL)
		throw CDDEException(CDDEException::E_REG_FAILED, ::DdeGetLastError(m_dwInst));
}

/******************************************************************************
** Method:		Unregister()
**
** Description:	Unregister the service name.
**
** Parameters:	pszService	The service name.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::Unregister(const tchar* pszService)
{
	ASSERT(pszService != NULL);

	CDDEString strService(this, pszService);

	::DdeNameService(m_dwInst, strService, NULL, DNS_UNREGISTER);
}

/******************************************************************************
** Method:		DestroyConversation()
**
** Description:	Disconnects a conversation.
**
** Parameters:	pConv	The conversation.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::DestroyConversation(CDDESvrConv* pConv)
{
	ASSERT(pConv != NULL);
	ASSERT(m_aoConvs.Find(pConv) != -1);

	// Disconnect from service/topic.
	pConv->Disconnect();

	// Remove from collection.
	m_aoConvs.Remove(m_aoConvs.Find(pConv));

	// Delete conversation.
	delete pConv;
}

/******************************************************************************
** Method:		FindConversation()
**
** Description:	Find an existing conversation.
**
** Parameters:	pszService	The service name.
**				pszTopic	The topic name.
**				hConv		The conversation handle.
**
** Returns:		The conversation or NULL if not found.
**
*******************************************************************************
*/

CDDESvrConv* CDDEServer::FindConversation(const tchar* pszService, const tchar* pszTopic) const
{
	ASSERT(pszService != NULL);
	ASSERT(pszTopic   != NULL);

	// Search the conversation list.
	for (size_t i = 0; i < m_aoConvs.Size(); ++i)
	{
		CDDESvrConv* pConv = m_aoConvs[i];

		if ( (pConv->Service() == pszService) && (pConv->Topic() == pszTopic) )
			return pConv;
	}

	return NULL;
}

CDDESvrConv* CDDEServer::FindConversation(HCONV hConv) const
{
	ASSERT(hConv != NULL);

	// Search the conversation list.
	for (size_t i = 0; i < m_aoConvs.Size(); ++i)
	{
		CDDESvrConv* pConv = m_aoConvs[i];

		if (pConv->Handle() == hConv)
			return pConv;
	}

	return NULL;
}


/******************************************************************************
** Method:		AddListener()
**
** Description:	Add a DDE Server event listener.
**
** Parameters:	pListener	The listener.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::AddListener(IDDEServerListener* pListener)
{
	ASSERT(pListener != NULL);

	m_aoListeners.Add(pListener);
}

/******************************************************************************
** Method:		RemoveListener()
**
** Description:	Remove a DDE Server event listener.
**
** Parameters:	pListener	The listener.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::RemoveListener(IDDEServerListener* pListener)
{
	ASSERT(pListener != NULL);

	int i = m_aoListeners.Find(pListener);

	if (i != -1)
		m_aoListeners.Remove(i);
}

/******************************************************************************
** Methods:		OnWildConnect*()
**
** Description:	Multiple connection request from a DDE Client. The DDE Client
**				will create a connection for each returned service & topic.
**				NB: It is accepted if any listener accepts it.
**
** Parameters:	pszService		The service name.
**				pszTopic		The topic name.
**				astrServices	The returned list of service names.
**				astrTopics		The returned list of topic names.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnWildConnect(CStrArray& astrServices, CStrArray& astrTopics)
{
	// Query all listeners...
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
	{
		if (m_aoListeners[i]->OnWildConnect(astrServices, astrTopics))
			return true;
	}

	return false;
}

bool CDDEServer::OnWildConnectService(const tchar* pszService, CStrArray& astrTopics)
{
	ASSERT(pszService != NULL);

	// Query all listeners...
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
	{
		if (m_aoListeners[i]->OnWildConnectService(pszService, astrTopics))
			return true;
	}

	return false;
}

bool CDDEServer::OnWildConnectTopic(const tchar* pszTopic, CStrArray& astrServices)
{
	ASSERT(pszTopic != NULL);

	// Query all listeners...
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
	{
		if (m_aoListeners[i]->OnWildConnectTopic(pszTopic, astrServices))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		OnConnect()
**
** Description:	Connection request from a DDE Client.
**				NB: It is accepted if any listener accepts it.
**
** Parameters:	pszService	The service name.
**				pszTopic	The topic name.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnConnect(const tchar* pszService, const tchar* pszTopic)
{
	ASSERT(pszService != NULL);
	ASSERT(pszTopic   != NULL);

	// Query all listeners...
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
	{
		if (m_aoListeners[i]->OnConnect(pszService, pszTopic))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		OnConnectConfirm()
**
** Description:	Connection established with a DDE Client.
**
** Parameters:	hConv		The conversation handle.
**				pszService	The service name.
**				pszTopic	The topic name.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::OnConnectConfirm(HCONV hConv, const tchar* pszService, const tchar* pszTopic)
{
	ASSERT(hConv      != NULL);
	ASSERT(pszService != NULL);
	ASSERT(pszTopic   != NULL);

	// Allocate a new conversation and add to the collection.
	CDDESvrConv* pConv = new CDDESvrConv(this, hConv, pszService, pszTopic);

	m_aoConvs.Add(pConv);

	// Notify listeners.
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
		m_aoListeners[i]->OnConnectConfirm(pConv);
}

/******************************************************************************
** Method:		OnDisconnect()
**
** Description:	Connection terminated with a DDE Client.
**
** Parameters:	hConv		The conversation handle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::OnDisconnect(HCONV hConv)
{
	ASSERT(hConv != NULL);

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Notify listeners.
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
		m_aoListeners[i]->OnDisconnect(pConv);

	// Remove from collection and delete.
	m_aoConvs.Remove(m_aoConvs.Find(pConv));
	delete pConv;
}

/******************************************************************************
** Method:		OnRequest()
**
** Description:	DDE Client requesting an item.
**
** Parameters:	hConv		The conversation handle.
**				pszItem		The item being requested.
**				nFormat		The data format.
**				oData		The data to return.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnRequest(HCONV hConv, const tchar* pszItem, uint nFormat, CDDEData& oData)
{
	ASSERT(hConv   != NULL);
	ASSERT(pszItem != NULL);

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Query all listeners...
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
	{
		if (m_aoListeners[i]->OnRequest(pConv, pszItem, nFormat, oData))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		OnAdviseStart()
**
** Description:	DDE Client requesting an advise on an item.
**
** Parameters:	hConv		The conversation handle.
**				pszItem		The item being requested.
**				nFormat		The data format.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnAdviseStart(HCONV hConv, const tchar* pszItem, uint nFormat)
{
	ASSERT(hConv   != NULL);
	ASSERT(pszItem != NULL);

	bool bAccept = false;

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Query all listeners...
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
	{
		if (m_aoListeners[i]->OnAdviseStart(pConv, pszItem, nFormat))
		{
			bAccept = true;
			break;
		}
	}

	// Accepted?
	if (bAccept)
	{
		// Create a link and add to conversation collection.
		CDDELink* pLink = pConv->CreateLink(pszItem, nFormat);

		// Notify listeners.
		for (size_t i = 0; i < m_aoListeners.Size(); ++i)
			m_aoListeners[i]->OnAdviseConfirm(pConv, pLink);
	}

	return bAccept;
}

/******************************************************************************
** Method:		OnAdviseRequest()
**
** Description:	DDEML requesting data for a link which has been updated.
**
** Parameters:	hConv		The conversation handle.
**				pszItem		The item being requested.
**				nFormat		The data format.
**				oData		The data to return.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnAdviseRequest(HCONV hConv, const tchar* pszItem, uint nFormat, CDDEData& oData)
{
	ASSERT(hConv   != NULL);
	ASSERT(pszItem != NULL);

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Find the conversations link.
	CDDELink* pLink = pConv->FindLink(pszItem, nFormat);

	ASSERT(pLink != NULL);

	// Notify listeners.
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
	{
		if (m_aoListeners[i]->OnAdviseRequest(pConv, pLink, oData))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		OnAdviseStop()
**
** Description:	Advise terminated on an item.
**
** Parameters:	hConv		The conversation handle.
**				pszItem		The item being requested.
**				nFormat		The data format.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::OnAdviseStop(HCONV hConv, const tchar* pszItem, uint nFormat)
{
	ASSERT(hConv   != NULL);
	ASSERT(pszItem != NULL);

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Find the conversations link.
	CDDELink* pLink = pConv->FindLink(pszItem, nFormat);

	ASSERT(pLink != NULL);

	// Notify listeners.
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
		m_aoListeners[i]->OnAdviseStop(pConv, pLink);

	// Remove from conversation.
	pConv->DestroyLink(pLink);
}

/******************************************************************************
** Method:		OnExecute()
**
** Description:	Execute a command.
**
** Parameters:	hConv		The conversation handle.
**				oData		The command.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnExecute(HCONV hConv, const CDDEData& oData)
{
	ASSERT(hConv != NULL);

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	ASSERT_FALSE(); // Is this comamnd string Ansi or Unicode or tchar?
	// Command data is a string.
#ifdef ANSI_BUILD
	CString strCmd = oData.GetString(ANSI_TEXT);
#else
	CString strCmd = oData.GetString(UNICODE_TEXT);
#endif

	// Notify listeners.
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
	{
		if (m_aoListeners[i]->OnExecute(pConv, strCmd))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		OnPoke()
**
** Description:	Poke a value into an item.
**
** Parameters:	hConv		The conversation handle.
**				pszItem		The item being poked.
**				nFormat		The data format.
**				oData		The data.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnPoke(HCONV hConv, const tchar* pszItem, uint nFormat, const CDDEData& oData)
{
	ASSERT(hConv != NULL);

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Notify listeners.
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
	{
		if (m_aoListeners[i]->OnPoke(pConv, pszItem, nFormat, oData))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		DDECallbackProc()
**
** Description:	The callback function used by the DDEML library.
**
** Parameters:	See DdeCallback.
**
** Returns:		See DdeCallback.
**
*******************************************************************************
*/

HDDEDATA CALLBACK CDDEServer::DDECallbackProc(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD /*dwData1*/, DWORD /*dwData2*/)
{
	ASSERT(g_pDDEServer != NULL);

	HDDEDATA hResult = NULL;

	// Forward to handler.
	switch (uType)
	{
		// Querying supported services/topics?
		case XTYP_WILDCONNECT:
		{
			// Querying all supported services & topics?
			if ((hsz2 == NULL) && (hsz1 == NULL))
			{
				CStrArray astrServices, astrTopics;

				// Get list of servers and topics.
				if (g_pDDEServer->OnWildConnect(astrServices, astrTopics))
				{
					ASSERT(astrServices.Size() > 0);
					ASSERT(astrServices.Size() == astrTopics.Size());

					int nEntries = (astrServices.Size() + 1);
					int nBufSize = sizeof(HSZPAIR) * nEntries;

					// Allocate temporary data buffer.
					HSZPAIR* aoPairs = static_cast<HSZPAIR*>(_alloca(nBufSize));

					// Set entries...
					for (size_t i = 0; i < astrServices.Size(); ++i)
					{
						aoPairs[i].hszSvc   = CDDEString(g_pDDEServer, astrServices[i], false);
						aoPairs[i].hszTopic = CDDEString(g_pDDEServer, astrTopics[i],   false);
					}

					// Last entry is NULL.
					aoPairs[nEntries-1].hszSvc   = NULL;
					aoPairs[nEntries-1].hszTopic = NULL;

					// Create result handle from temporary buffer.
					hResult = CDDEData(g_pDDEServer, aoPairs, nBufSize, 0, 0, false).Handle();
				}
			}
			// Querying all topics supported by a specific server?
			else if (hsz1 == NULL)
			{
				CDDEString strService(g_pDDEServer, hsz2);

				CStrArray astrTopics;

				// Get list of topics.
				if (g_pDDEServer->OnWildConnectService(strService, astrTopics))
				{
					ASSERT(astrTopics.Size() > 0);

					int nEntries = (astrTopics.Size() + 1);
					int nBufSize = sizeof(HSZPAIR) * nEntries;

					// Allocate temporary data buffer.
					HSZPAIR* aoPairs = static_cast<HSZPAIR*>(_alloca(nBufSize));

					// Set entries...
					for (size_t i = 0; i < astrTopics.Size(); ++i)
					{
						aoPairs[i].hszSvc   = hsz2;
						aoPairs[i].hszTopic = CDDEString(g_pDDEServer, astrTopics[i],   false);
					}

					// Last entry is NULL.
					aoPairs[nEntries-1].hszSvc   = NULL;
					aoPairs[nEntries-1].hszTopic = NULL;

					// Create result handle from temporary buffer.
					hResult = CDDEData(g_pDDEServer, aoPairs, nBufSize, 0, 0, false).Handle();
				}
			}
			// Querying all servers supporting a specific topic?
			else if (hsz2 == NULL)
			{
				CDDEString strTopic(g_pDDEServer, hsz1);

				CStrArray astrServices, astrTopics;

				// Get list of servers.
				if (g_pDDEServer->OnWildConnectTopic(strTopic, astrTopics))
				{
					ASSERT(astrServices.Size() > 0);

					int nEntries = (astrServices.Size() + 1);
					int nBufSize = sizeof(HSZPAIR) * nEntries;

					// Allocate temporary data buffer.
					HSZPAIR* aoPairs = static_cast<HSZPAIR*>(_alloca(nBufSize));

					// Set entries...
					for (size_t i = 0; i < astrTopics.Size(); ++i)
					{
						aoPairs[i].hszSvc   = CDDEString(g_pDDEServer, astrServices[i], false);
						aoPairs[i].hszTopic = hsz1;
					}

					// Last entry is NULL.
					aoPairs[nEntries-1].hszSvc   = NULL;
					aoPairs[nEntries-1].hszTopic = NULL;

					// Create result handle from temporary buffer.
					hResult = CDDEData(g_pDDEServer, aoPairs, nBufSize, 0, 0, false).Handle();
				}
			}
		}
		break;

		// Connection requested?
		case XTYP_CONNECT:
		{
			CDDEString strService(g_pDDEServer, hsz2);
			CDDEString strTopic(g_pDDEServer, hsz1);

			hResult = (HDDEDATA) g_pDDEServer->OnConnect(strService, strTopic);
		}
		break;

		// Connection established?
		case XTYP_CONNECT_CONFIRM:
		{
			CDDEString strService(g_pDDEServer, hsz2);
			CDDEString strTopic(g_pDDEServer, hsz1);

			g_pDDEServer->OnConnectConfirm(hConv, strService, strTopic);
		}
		break;

		// Connection terminated?
		case XTYP_DISCONNECT:
		{
			g_pDDEServer->OnDisconnect(hConv);
		}
		break;

		// Item request?
		case XTYP_REQUEST:
		{
			CDDEString strItem(g_pDDEServer, hsz2);
			CDDEData   oData(g_pDDEServer, hsz2, uFormat, false);

			if (g_pDDEServer->OnRequest(hConv, strItem, uFormat, oData))
				hResult = oData.Handle();
			else
				oData.Free();
		}
		break;

		// Advise requested?
		case XTYP_ADVSTART:
		{
			CDDEString strItem(g_pDDEServer, hsz2);

			hResult = (HDDEDATA) g_pDDEServer->OnAdviseStart(hConv, strItem, uFormat);
		}
		break;

		// Item updated?
		case XTYP_ADVREQ:
		{
			CDDEString strItem(g_pDDEServer, hsz2);
			CDDEData   oData(g_pDDEServer, hsz2, uFormat, false);

			if (g_pDDEServer->OnAdviseRequest(hConv, strItem, uFormat, oData))
				hResult = oData.Handle();
			else
				oData.Free();
		}
		break;

		// Advise terminated?
		case XTYP_ADVSTOP:
		{
			CDDEString strItem(g_pDDEServer, hsz2);

			g_pDDEServer->OnAdviseStop(hConv, strItem, uFormat);
		}
		break;

		// Execute command?
		case XTYP_EXECUTE:
		{
			CDDEData oData(g_pDDEServer, hData, CF_TEXT, true);

			hResult = DDE_FNOTPROCESSED;

			if (g_pDDEServer->OnExecute(hConv, oData))
				hResult = (HDDEDATA) DDE_FACK;
		}
		break;

		// Poke item?
		case XTYP_POKE:
		{
			CDDEString strItem(g_pDDEServer, hsz2);
			CDDEData   oData(g_pDDEServer, hData, uFormat, false);

			hResult = DDE_FNOTPROCESSED;

			if (g_pDDEServer->OnPoke(hConv, strItem, uFormat, oData))
				hResult = (HDDEDATA) DDE_FACK;
		}
		break;

		// Unknown message.
		default:
		{
			TRACE1(TXT("DDECallbackProc(0x%08X)\n"), uType);
			ASSERT_FALSE();
		}
		break;
	}

	return hResult;
}
