////////////////////////////////////////////////////////////////////////////////
//! \file   ClassGenApp.cpp
//! \brief  The ClassGenApp class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ClassGenApp.hpp"
#include <WCL/FileException.hpp>
#include <Core/StringUtils.hpp>

////////////////////////////////////////////////////////////////////////////////
// Global variables.

//! The application object.
CClassGenApp g_app;

////////////////////////////////////////////////////////////////////////////////
// Constants.

const tchar* TEMPLATES_FILE	    = TXT("Templates.ini");
const tchar* TEMPLATES_FILE_VER = TXT("2.0");
const tchar* SETTINGS_FILE_VER  = TXT("2.0");

////////////////////////////////////////////////////////////////////////////////
//! Default constructor

CClassGenApp::CClassGenApp()
	: CApp(m_appWnd, m_appCmds)
	, m_strTmplFolder(TXT("."))
	, m_strHppExt(TXT(".hpp"))
	, m_strCppExt(TXT(".cpp"))
	, m_author(TXT("The Author"))
{

}

////////////////////////////////////////////////////////////////////////////////
//! Destructor

CClassGenApp::~CClassGenApp()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Handle application startup.

bool CClassGenApp::OnOpen()
{
	// Set the app title.
	m_strTitle = TXT("Class Generator");

	// Load settings.
	loadConfig();
	
	// Create the main window.
	if (!m_appWnd.Create())
		return false;

	// Show it.
	m_appWnd.Centre();
	m_appWnd.Show(m_iCmdShow);

	// Update UI.
	m_appCmds.UpdateUI();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle application shutdown.

bool CClassGenApp::OnClose()
{
	// Save settings.
	saveConfig();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Load the application settings.

void CClassGenApp::loadConfig()
{
	CIniFile templatesFile(CPath::ApplicationDir() / TEMPLATES_FILE);

	// Read the templates file version.
	CString templatesVer = templatesFile.ReadString(TXT("Version"), TXT("Version"), TEMPLATES_FILE_VER);

	if (templatesVer != TEMPLATES_FILE_VER)
		throw CFileException(CStreamException::E_VERSION_INVALID, templatesFile.m_strPath, NO_ERROR);

	// Read the templates settings.
	m_strTmplFolder = templatesFile.ReadString(TXT("Templates"), TXT("Path"), m_strTmplFolder);
	m_strHppExt = templatesFile.ReadString(TXT("Templates"), TXT("HppExt"), m_strHppExt);
	m_strCppExt = templatesFile.ReadString(TXT("Templates"), TXT("CppExt"), m_strCppExt);
	m_author = templatesFile.ReadString(TXT("Templates"), TXT("Author"), m_author);

	// Read the template names.
	size_t count = templatesFile.ReadInt(TXT("Templates"), TXT("Count"), 0);

	for (size_t i = 0; i < count; ++i)
	{
		tstring section = Core::Fmt(TXT("Template[%u]"), i);
		tstring name    = templatesFile.ReadString(TXT("Templates"), section.c_str(), tstring());

		// Valid template name?
		if (!name.empty())
		{
			TemplatePtr item(new Template);
			tstring     section = name + TXT(" Template");

			item->m_name        = name;
			item->m_description = templatesFile.ReadString(section, TXT("Description"), TXT(""));
			item->m_isClass     = templatesFile.ReadBool  (section, TXT("Class"), true);
			item->m_headerFile  = templatesFile.ReadString(section, TXT("HPP"), TXT(""));
			item->m_sourceFile  = templatesFile.ReadString(section, TXT("CPP"), TXT(""));

			m_templates.push_back(item);
		}
	}

	// Read the component names.
	count = templatesFile.ReadInt(TXT("Components"), TXT("Count"), 0);

	for (size_t i = 0; i < count; ++i)
	{
		tstring section = Core::Fmt(TXT("Component[%d]"), i);
		tstring name    = templatesFile.ReadString(TXT("Components"), section, tstring());

		// Valid component name?
		if (!name.empty())
		{
			ComponentPtr item(new Component);
			tstring     section = name + TXT(" Component");

			item->m_name        = name;
			item->m_description = templatesFile.ReadString(section, TXT("Description"), TXT(""));
			item->m_include     = templatesFile.ReadString(section, TXT("Include"),   TXT(""));
			item->m_comment     = templatesFile.ReadString(section, TXT("Comment"),   TXT(""));
			item->m_namespace   = templatesFile.ReadString(section, TXT("Namespace"), TXT(""));
			item->m_folder      = templatesFile.ReadString(section, TXT("Folder"),    TXT(""));

			m_components.push_back(item);
		}
	}

	CIniFile settingsFile;

	// Read the templates file version.
	CString settingsVer = settingsFile.ReadString(TXT("Version"), TXT("Version"), SETTINGS_FILE_VER);

	if (settingsVer != SETTINGS_FILE_VER)
		throw CFileException(CStreamException::E_VERSION_INVALID, settingsFile.m_strPath, NO_ERROR);

	// Read the list of folders used.
	int nFolders = settingsFile.ReadInt(TXT("Folders"), TXT("Count"), 0);

	for (int i = 0; i < nFolders; ++i)
	{
		CString strSection;

		strSection.Format(TXT("Folder[%d]"), i);

		CString strFolder = settingsFile.ReadString(TXT("Folders"), strSection, TXT(""));

		if ( (!strFolder.Empty()) && (m_astrFolders.Find(strFolder, true) == -1) )
			m_astrFolders.Add(strFolder);
	}

	// Read the last use settings.
	m_strLastComponent = settingsFile.ReadString(TXT("Main"), TXT("LastComponent"), m_strLastComponent);
	m_strLastFolder    = settingsFile.ReadString(TXT("Main"), TXT("LastFolder"), m_strLastFolder);
}

////////////////////////////////////////////////////////////////////////////////
//! Save the application settings.

void CClassGenApp::saveConfig()
{
	CIniFile settingsFile;

	// Write the file version.
	settingsFile.WriteString(TXT("Version"), TXT("Version"), SETTINGS_FILE_VER);

	// Write the list of folders used.
	settingsFile.WriteInt(TXT("Folders"), TXT("Count"), m_astrFolders.Size());

	for (size_t i = 0; i < m_astrFolders.Size(); ++i)
	{
		CString strSection;

		strSection.Format(TXT("Folder[%d]"), i);

		settingsFile.WriteString(TXT("Folders"), strSection, m_astrFolders[i]);
	}

	// Write the last use settings.
	settingsFile.WriteString(TXT("Main"), TXT("LastComponent"), m_strLastComponent);
	settingsFile.WriteString(TXT("Main"), TXT("LastFolder"), m_strLastFolder);
}
