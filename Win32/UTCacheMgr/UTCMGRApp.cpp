/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UTCMGRAPP.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CUTCMGRApp class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// "The" application object.
CUTCMGRApp App;

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

#ifdef _DEBUG
const char* CUTCMGRApp::VERSION      = "v1.1 Beta [Debug]";
#else
const char* CUTCMGRApp::VERSION      = "v1.1 Beta";
#endif
const char* CUTCMGRApp::INI_FILE_VER = "1.1";

/******************************************************************************
** Method:		Constructor
**
** Description:	Default constructor.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CUTCMGRApp::CUTCMGRApp()
	: CApp(m_AppWnd, m_AppCmds)
	, m_oHelpFile(m_AppWnd)
	, m_oCache(m_oMDB)
	, m_pProfile(NULL)
	, m_bScanOnStart(true)
	, m_bScanOnSwitch(true)
	, m_bScanForTmp(true)
	, m_bScanIndex(true)
	, m_bShowAllFiles(false)
	, m_bLogEdits(true)
{
	// Create the MDB.
	m_oMDB.AddTable(m_oCache);

	// Set the help file path.
	m_oHelpFile.m_HelpFile  = CPath(CPath::ApplicationDir(), "UTCacheMgr.hlp");
}

/******************************************************************************
** Method:		Destructor
**
** Description:	Cleanup.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CUTCMGRApp::~CUTCMGRApp()
{
	m_aoProfiles.DeleteAll();
}

/******************************************************************************
** Method:		OnOpen()
**
** Description:	Initialises the application.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CUTCMGRApp::OnOpen()
{
	// Set the app title.
	m_strTitle = "UT Cache Manager";

	// Load settings.
	LoadConfig();
	
	// Load the toolbar bitmap.
	m_rCmdControl.CmdBitmap().LoadRsc(IDR_APPTOOLBAR);

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	if ( (m_iCmdShow == SW_SHOWNORMAL) && (m_rcLastPos.Empty() == false) )
		m_AppWnd.Move(m_rcLastPos);

	m_AppWnd.Show(m_iCmdShow);

	// Update UI.
	m_AppCmds.UpdateUI();

	// Scan on startup?
	if (m_bScanOnStart)
		m_AppWnd.PostCommand(ID_CACHE_RESCAN);

	return true;
}

/******************************************************************************
** Method:		OnClose()
**
** Description:	Cleans up the application resources.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CUTCMGRApp::OnClose()
{
	// Save settings.
	SaveConfig();

	return true;
}

/******************************************************************************
** Method:		LoadConfig()
**
** Description:	Load the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTCMGRApp::LoadConfig()
{
	// Read the file version.
	CString strVer = m_oIniFile.ReadString("Version", "Version", INI_FILE_VER);

	// Read the cache general settings.
	m_strCacheIndex = m_oIniFile.ReadString("Cache", "Index",        CProfile::DEF_CACHE_IDX_FILE);
	m_strCacheExt   = m_oIniFile.ReadString("Cache", "Ext",          CProfile::DEF_CACHE_FILE_EXT);
	m_bScanOnStart  = m_oIniFile.ReadBool  ("Cache", "ScanOnStart",  m_bScanOnStart);
	m_bScanOnSwitch = m_oIniFile.ReadBool  ("Cache", "ScanOnSwitch", m_bScanOnSwitch);
	m_bScanForTmp   = m_oIniFile.ReadBool  ("Cache", "ScanForTmp",   m_bScanForTmp);
	m_bScanIndex    = m_oIniFile.ReadBool  ("Cache", "ScanIndex",    m_bScanIndex);
	m_bShowAllFiles = m_oIniFile.ReadBool  ("Cache", "ShowAllFiles", m_bShowAllFiles);
	m_bLogEdits     = m_oIniFile.ReadBool  ("Cache", "LogEdits",     m_bLogEdits);
	m_strLastCopyTo = m_oIniFile.ReadString("Cache", "LastCopyTo",   "");

	// Read the number of profiles.
	uint nProfiles  = m_oIniFile.ReadInt("Profiles", "Count",   0);

	// Read the profiles.
	for (uint i = 0; i < nProfiles; ++i)
	{
		CString  strSection;
		CProfile oProfile;

		// Create section name.
		strSection.Format("Profile%d", i);

		// Read profile details.
		oProfile.m_strName       = m_oIniFile.ReadString(strSection, "Name",       "");
		oProfile.m_strCacheDir   = m_oIniFile.ReadString(strSection, "CacheDir",   "");
		oProfile.m_bReadOnly     = m_oIniFile.ReadBool  (strSection, "ReadOnly",   false);
		oProfile.m_strSystemDir  = m_oIniFile.ReadString(strSection, "SystemDir",  "");
		oProfile.m_strMapDir     = m_oIniFile.ReadString(strSection, "MapDir",     "");
		oProfile.m_strTextureDir = m_oIniFile.ReadString(strSection, "TextureDir", "");
		oProfile.m_strSoundDir   = m_oIniFile.ReadString(strSection, "SoundDir",   "");
		oProfile.m_strMusicDir   = m_oIniFile.ReadString(strSection, "MusicDir",   "");
		oProfile.m_strConfigFile = m_oIniFile.ReadString(strSection, "ConfigFile", "");

		// If valid, add to collection.
		if (oProfile.m_strName.Length() > 0)
			m_aoProfiles.Add(new CProfile(oProfile));
	}

	// Read the default profile.
	m_strDefProfile = m_oIniFile.ReadString("Profiles", "Default", "Default");

	// Find the default profile.
	if (m_pProfile == NULL)
		m_pProfile = FindProfile(m_strDefProfile);

	if (m_pProfile == NULL)
		m_pProfile = FindProfile(CProfile::DEF_PROFILE_NAME);

	// Create a default one.
	if (m_pProfile == NULL)
	{
		m_pProfile = new CProfile();

		// Initialise profile with sensible defaults.
		m_pProfile->m_strName       = CProfile::DEF_PROFILE_NAME;
		m_pProfile->m_strCacheDir   = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_CACHE_DIR   );
		m_pProfile->m_bReadOnly     = false;
		m_pProfile->m_strSystemDir  = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_SYSTEM_DIR  );
		m_pProfile->m_strMapDir     = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_MAPS_DIR    );
		m_pProfile->m_strTextureDir = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_TEXTURES_DIR);
		m_pProfile->m_strSoundDir   = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_SOUNDS_DIR  );
		m_pProfile->m_strMusicDir   = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_MUSIC_DIR   );
		m_pProfile->m_strConfigFile = CPath(m_pProfile->m_strSystemDir, CProfile::DEF_CONFIG_FILE );

		CRegKey oUTKey;

		// Try and find the regkey that contains the UT base path.
		if (oUTKey.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Unreal Technology\\Installed Apps\\UnrealTournament"))
		{
			// Get the UT base path.
			CPath strBaseDir = oUTKey.QueryString("Folder", "");

			if (strBaseDir != "")
			{
				// Re-initialise profile with correct paths.
				m_pProfile->m_strCacheDir   = CPath(strBaseDir, CProfile::DEF_CACHE_DIR   );
				m_pProfile->m_strSystemDir  = CPath(strBaseDir, CProfile::DEF_SYSTEM_DIR  );
				m_pProfile->m_strMapDir     = CPath(strBaseDir, CProfile::DEF_MAPS_DIR    );
				m_pProfile->m_strTextureDir = CPath(strBaseDir, CProfile::DEF_TEXTURES_DIR);
				m_pProfile->m_strSoundDir   = CPath(strBaseDir, CProfile::DEF_SOUNDS_DIR  );
				m_pProfile->m_strMusicDir   = CPath(strBaseDir, CProfile::DEF_MUSIC_DIR   );
				m_pProfile->m_strConfigFile = CPath(m_pProfile->m_strSystemDir, CProfile::DEF_CONFIG_FILE);
			}
		}

		// Add to the profiles collection.
		m_aoProfiles.Add(m_pProfile);
	}

	ASSERT(m_pProfile != NULL);

	// Update the default profile.
	m_strDefProfile = m_pProfile->m_strName;

	// Read the number of pinned files.
	uint nPinned = m_oIniFile.ReadInt("Pinned", "Count",   0);

	// Read the pinned files.
	for (i = 0; i < nPinned; ++i)
	{
		CString strEntry;
		CString strName;

		// Create entry name.
		strEntry.Format("File%d", i);

		strName = m_oIniFile.ReadString("Pinned", strEntry, "");

		// Add if valid and not already listed.
		if ((strName.Length() > 0) && (m_astrPinned.Find(strName, false) == -1))
			m_astrPinned.Add(strName);
	}

	// Read the window pos and size.
	m_rcLastPos.left   = m_oIniFile.ReadInt("UI", "Left",   0);
	m_rcLastPos.top    = m_oIniFile.ReadInt("UI", "Top",    0);
	m_rcLastPos.right  = m_oIniFile.ReadInt("UI", "Right",  0);
	m_rcLastPos.bottom = m_oIniFile.ReadInt("UI", "Bottom", 0);
}

/******************************************************************************
** Method:		SaveConfig()
**
** Description:	Save the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTCMGRApp::SaveConfig()
{
	// Write the file version.
	m_oIniFile.WriteString("Version", "Version", INI_FILE_VER);

	// Write the cache general settings.
	m_oIniFile.WriteString("Cache", "Index",        m_strCacheIndex);
	m_oIniFile.WriteString("Cache", "Ext",          m_strCacheExt);
	m_oIniFile.WriteBool  ("Cache", "ScanOnStart",  m_bScanOnStart);
	m_oIniFile.WriteBool  ("Cache", "ScanOnSwitch", m_bScanOnSwitch);
	m_oIniFile.WriteBool  ("Cache", "ScanForTmp",   m_bScanForTmp);
	m_oIniFile.WriteBool  ("Cache", "ScanIndex",    m_bScanIndex);
	m_oIniFile.WriteBool  ("Cache", "ShowAllFiles", m_bShowAllFiles);
	m_oIniFile.WriteBool  ("Cache", "LogEdits",     m_bLogEdits);
	m_oIniFile.WriteString("Cache", "LastCopyTo",   m_strLastCopyTo);

	// Write the profiles.
	m_oIniFile.WriteString("Profiles", "Default", m_strDefProfile    );
	m_oIniFile.WriteInt   ("Profiles", "Count",   m_aoProfiles.Size());

	for (int i = 0; i < m_aoProfiles.Size(); ++i)
	{
		CString   strSection;
		CProfile* pProfile = m_aoProfiles[i];

		// Create section name.
		strSection.Format("Profile%d", i);

		// Write profile details.
		m_oIniFile.WriteString(strSection, "Name",       pProfile->m_strName      );
		m_oIniFile.WriteString(strSection, "CacheDir",   pProfile->m_strCacheDir  );
		m_oIniFile.WriteBool  (strSection, "ReadOnly",   pProfile->m_bReadOnly    );
		m_oIniFile.WriteString(strSection, "SystemDir",  pProfile->m_strSystemDir );
		m_oIniFile.WriteString(strSection, "MapDir",     pProfile->m_strMapDir    );
		m_oIniFile.WriteString(strSection, "TextureDir", pProfile->m_strTextureDir);
		m_oIniFile.WriteString(strSection, "SoundDir",   pProfile->m_strSoundDir  );
		m_oIniFile.WriteString(strSection, "MusicDir",   pProfile->m_strMusicDir  );
		m_oIniFile.WriteString(strSection, "ConfigFile", pProfile->m_strConfigFile);
	}

	// Write the list of pinned files.
	m_oIniFile.DeleteSection("Pinned");
	m_oIniFile.WriteInt ("Pinned", "Count", m_astrPinned.Size());

	for (i = 0; i < m_astrPinned.Size(); ++i)
	{
		CString strEntry;

		// Create entry name.
		strEntry.Format("File%d", i);

		m_oIniFile.WriteString("Pinned", strEntry, m_astrPinned[i]);
	}

	// Write the window pos and size.
	m_oIniFile.WriteInt("UI", "Left",   m_rcLastPos.left  );
	m_oIniFile.WriteInt("UI", "Top",    m_rcLastPos.top   );
	m_oIniFile.WriteInt("UI", "Right",  m_rcLastPos.right );
	m_oIniFile.WriteInt("UI", "Bottom", m_rcLastPos.bottom);
}

/******************************************************************************
** Method:		FindProfile()
**
** Description:	Finds a profile by name.
**
** Parameters:	None.
**
** Returns:		The profile or NULL.
**
*******************************************************************************
*/

CProfile* CUTCMGRApp::FindProfile(const char* pszName) const
{
	ASSERT(pszName != NULL);

	// For all profiles...
	for (int i = 0; i < m_aoProfiles.Size(); ++i)
	{
		if (m_aoProfiles[i]->m_strName.Compare(pszName, true) == 0)
			return m_aoProfiles[i];
	}

	return NULL;
}

/******************************************************************************
** Method:		FormatType()
**
** Description:	Convert the file type to a string.
**
** Parameters:	cType	The file type.
**
** Returns:		The type as a string.
**
*******************************************************************************
*/

CString CUTCMGRApp::FormatType(char cType) const
{
	switch (cType)
	{
		case SYSTEM_FILE :	return "System";
		case MAP_FILE    :	return "Map";
		case TEXTURE_FILE:	return "Texture";
		case SOUND_FILE  :	return "Sound";
		case MUSIC_FILE  :	return "Music";
	}

	ASSERT(false);

	return "";
}

/******************************************************************************
** Method:		FormatSize()
**
** Description:	Convert the file size to a string.
**
** Parameters:	nSize	The file size.
**
** Returns:		The size as a string.
**
*******************************************************************************
*/

CString CUTCMGRApp::FormatSize(int nSize) const
{
	CString str;

	// Ensure we report at least 1K.
	nSize = max(1024, nSize);

	str.Format("%d K", nSize/1024);

	return str;
}

/******************************************************************************
** Method:		FormatStatus()
**
** Description:	Convert the file status to a string.
**
** Parameters:	cStatus		The file status.
**
** Returns:		The status as a string.
**
*******************************************************************************
*/

CString CUTCMGRApp::FormatStatus(char cStatus) const
{
	switch (cStatus)
	{
		case NEW_FILE :	return "New";
		case OLD_FILE :	return "Old";
		case PIN_FILE :	return "Pinned";
	}

	ASSERT(false);

	return "";
}
