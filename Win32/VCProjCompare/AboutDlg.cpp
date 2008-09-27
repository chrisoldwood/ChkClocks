////////////////////////////////////////////////////////////////////////////////
//! \file   AboutDlg.cpp
//! \brief  The AboutDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AboutDlg.hpp"
#include "TheApp.hpp"
#include <WCL/Path.hpp>
#include <WCL/VerInfoReader.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AboutDlg::AboutDlg()
	: CDialog(IDD_ABOUT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_VERSION,	&m_versionLabel)
		CTRL(IDC_COPYRIGHT,	&m_copyrightLabel)
		CTRL(IDC_EMAIL,		&m_emailLabel)
		CTRL(IDC_WEBSITE,	&m_websiteLabel)
	END_CTRL_TABLE

	// Set the URL label protocols.
	m_emailLabel.Protocol(TXT("mailto:"));
	m_websiteLabel.Protocol(TXT("http://"));
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AboutDlg::~AboutDlg()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Handle dialog creation.

void AboutDlg::OnInitDialog()
{
	// Extract details from the resources.
	tstring filename  = CPath::Application();
	tstring version   = WCL::VerInfoReader::GetStringValue(filename, WCL::VerInfoReader::PRODUCT_VERSION);
	tstring copyright = WCL::VerInfoReader::GetStringValue(filename, WCL::VerInfoReader::LEGAL_COPYRIGHT);

#ifdef _DEBUG
	version += TXT(" [Debug]");
#endif

	// Update UI.
	m_versionLabel.setText(version);
	m_copyrightLabel.setText(copyright);
}
