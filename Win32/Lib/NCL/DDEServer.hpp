/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDESERVER.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEServer class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDESERVER_HPP
#define DDESERVER_HPP

// Template shorthands.
typedef TPtrArray<CDDESvrConv> CDDESvrConvs;

/******************************************************************************
** 
** This class provides DDE Server services.
**
*******************************************************************************
*/

class CDDEServer : public CDDEInst
{
public:
	virtual ~CDDEServer();

	//
	// Singleton methods.
	//
	static CDDEServer* Instance();

	//
	// Accessors.
	//

	//
	// Initialisation methods.
	//
	void Initialise(DWORD dwFlags = CBF_SKIP_REGISTRATIONS | CBF_SKIP_UNREGISTRATIONS);
	void Uninitialise();
	void Register(const char* pszService);
	void Unregister(const char* pszService);

	//
	// Conversation methods.
	//
	void         DestroyConversation(CDDESvrConv* pConv);
	CDDESvrConv* FindConversation(const char* pszService, const char* pszTopic) const;
	CDDESvrConv* FindConversation(HCONV hConv) const;
	int          GetNumConversations() const;
	int          GetAllConversations(CDDESvrConvs& aoConvs) const;

	//
	// Event listener methods.
	//
	void AddListener(IDDEServerListener* pListener);
	void RemoveListener(IDDEServerListener* pListener);

protected:
	// Template shorthands.
	typedef TPtrArray<IDDEServerListener> CListeners;

	//
	// Members.
	//
	CDDESvrConvs	m_aoConvs;		// The list of conversations.
	CListeners		m_aoListeners;	// The list of event listeners.

	//
	// Constructors/Destructor.
	//
	CDDEServer();
	CDDEServer(const CDDEServer&);

	//
	// DDECallback handlers.
	//
	bool OnWildConnect(CStrArray& astrServices, CStrArray& astrTopics);
	bool OnWildConnectService(const char* pszService, CStrArray& astrTopics);
	bool OnWildConnectTopic(const char* pszTopic, CStrArray& astrServices);
	bool OnConnect(const char* pszService, const char* pszTopic);
	void OnConnectConfirm(HCONV hConv, const char* pszService, const char* pszTopic);
	void OnDisconnect(HCONV hConv);
	bool OnRequest(HCONV hConv, const char* pszItem, uint nFormat, CDDEData& oData);
	bool OnAdviseStart(HCONV hConv, const char* pszItem, uint nFormat);
	bool OnAdviseRequest(HCONV hConv, const char* pszItem, uint nFormat, CDDEData& oData);
	void OnAdviseStop(HCONV hConv, const char* pszItem, uint nFormat);
	bool OnExecute(HCONV hConv, const CDDEData& oData);
	bool OnPoke(HCONV hConv, const char* pszItem, uint nFormat, const CDDEData& oData);

	// The DDE Callback function.
	static HDDEDATA CALLBACK DDECallbackProc(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2);

	// The single DDE Client object.
	static CDDEServer* g_pDDEServer;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CDDEServer::GetNumConversations() const
{
	return m_aoConvs.Size();
}

inline int CDDEServer::GetAllConversations(CDDESvrConvs& aoConvs) const
{
	aoConvs.ShallowCopy(m_aoConvs);

	return aoConvs.Size();
}

#endif // DDESERVER_HPP