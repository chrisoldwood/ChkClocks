/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDESERVER.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDEServer class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

/******************************************************************************
**
** Class members.
**
*******************************************************************************
*/

// The single DDE Client object.
CDDEServer* CDDEServer::g_pDDEServer = NULL;

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

CDDEServer::CDDEServer()
{
	m_eType = SERVER;
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

CDDEServer::~CDDEServer()
{
	Uninitialise();

	ASSERT(m_aoConvs.Size() == 0);
	ASSERT(m_aoListeners.Size() == 0);
}

/******************************************************************************
** Method:		Instance()
**
** Description:	Get the single instance of the DDE Server.
**
** Parameters:	None.
**
** Returns:		The DDE Client.
**
*******************************************************************************
*/

CDDEServer* CDDEServer::Instance()
{
	static CDDEServer g_oDDEServer;

	// Set Callback object pointer.
	g_pDDEServer = &g_oDDEServer;

	return g_pDDEServer;
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
	for (int i = 0; i < m_aoConvs.Size(); ++i)
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

void CDDEServer::Register(const char* pszService)
{
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

void CDDEServer::Unregister(const char* pszService)
{
	CDDEString strService(this, pszService);

	::DdeNameService(m_dwInst, strService, NULL, DNS_UNREGISTER);
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

CDDESvrConv* CDDEServer::FindConversation(const char* pszService, const char* pszTopic)
{
	ASSERT(pszService != NULL);
	ASSERT(pszTopic   != NULL);

	// Search the conversation list.
	for (int i = 0; i < m_aoConvs.Size(); ++i)
	{
		CDDESvrConv* pConv = m_aoConvs[i];

		if ( (pConv->Service() == pszService) && (pConv->Topic() == pszTopic) )
			return pConv;
	}

	return NULL;
}

CDDESvrConv* CDDEServer::FindConversation(HCONV hConv)
{
	// Search the conversation list.
	for (int i = 0; i < m_aoConvs.Size(); ++i)
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
** Method:		OnConnect()
**
** Description:	Connection request from DDE Client.
**				NB: It is accepted if any listener accepts it.
**
** Parameters:	pszService	The service name.
**				pszTopic	The topic name.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnConnect(const char* pszService, const char* pszTopic)
{
	// Query all listeners...
	for (int i = 0; i < m_aoListeners.Size(); ++i)
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

void CDDEServer::OnConnectConfirm(HCONV hConv, const char* pszService, const char* pszTopic)
{
	// Allocate a new conversation and add to the collection.
	CDDESvrConv* pConv = new CDDESvrConv(this, hConv, pszService, pszTopic);

	m_aoConvs.Add(pConv);

	// Notify listeners.
	for (int i = 0; i < m_aoListeners.Size(); ++i)
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
	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Notify listeners.
	for (int i = 0; i < m_aoListeners.Size(); ++i)
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

bool CDDEServer::OnRequest(HCONV hConv, const char* pszItem, uint nFormat, CDDEData& oData)
{
	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Query all listeners...
	for (int i = 0; i < m_aoListeners.Size(); ++i)
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

bool CDDEServer::OnAdviseStart(HCONV hConv, const char* pszItem, uint nFormat)
{
	bool bAccept = false;

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Query all listeners...
	for (int i = 0; i < m_aoListeners.Size(); ++i)
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
		for (int i = 0; i < m_aoListeners.Size(); ++i)
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

bool CDDEServer::OnAdviseRequest(HCONV hConv, const char* pszItem, uint nFormat, CDDEData& oData)
{
	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Find the conversations link.
	CDDELink* pLink = pConv->FindLink(pszItem, nFormat);

	ASSERT(pLink != NULL);

	// Notify listeners.
	for (int i = 0; i < m_aoListeners.Size(); ++i)
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

void CDDEServer::OnAdviseStop(HCONV hConv, const char* pszItem, uint nFormat)
{
	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Find the conversations link.
	CDDELink* pLink = pConv->FindLink(pszItem, nFormat);

	ASSERT(pLink != NULL);

	// Notify listeners.
	for (int i = 0; i < m_aoListeners.Size(); ++i)
		m_aoListeners[i]->OnAdviseStop(pConv, pLink);

	// Remove from conversation.
	pConv->DestroyLink(pLink);
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

HDDEDATA CALLBACK CDDEServer::DDECallbackProc(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2)
{
	ASSERT(g_pDDEServer != NULL);

	HDDEDATA hResult = NULL;

	// Forward to handler.
	switch (uType)
	{
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
			CDDEData   oData(g_pDDEServer, hsz2, uFormat);

			hResult = oData;

			if (!g_pDDEServer->OnRequest(hConv, strItem, uFormat, oData))
			{
				::DdeFreeDataHandle(oData);
				hResult = NULL;
			}
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
			CDDEData   oData(g_pDDEServer, hsz2, uFormat);

			hResult = oData;

			if (!g_pDDEServer->OnAdviseRequest(hConv, strItem, uFormat, oData))
			{
				::DdeFreeDataHandle(oData);
				hResult = NULL;
			}
		}
		break;

		// Advise terminated?
		case XTYP_ADVSTOP:
		{
			CDDEString strItem(g_pDDEServer, hsz2);

			g_pDDEServer->OnAdviseStop(hConv, strItem, uFormat);
		}
		break;

		// Unknown message.
		default:
		{
			TRACE1("DDECallbackProc(0x%08X)\n", uType);
			ASSERT(false);
		}
		break;
	}

	return hResult;
}