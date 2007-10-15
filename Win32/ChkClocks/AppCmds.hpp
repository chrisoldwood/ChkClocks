/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppCmds class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPCMDS_HPP
#define APPCMDS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CmdCtrl.hpp>

/******************************************************************************
** 
** Application command controller.
**
*******************************************************************************
*/

class CAppCmds : public CCmdControl
{
public:
	//
	// Constructors/Destructor.
	//
	CAppCmds();
	~CAppCmds();

	//
	// Commands.
	//

	// File menu.
	void OnFileCheck();
	void OnFileExit();

	// Options menu.
	void OnReportClipboard();
	void OnReportFile();
	void OnReportPrint();

	// Options menu.
	void OnOptionsScan();
	void OnOptionsReport();

	// Help menu.
	void OnHelpAbout();

	// Context menu.
	void OnExcludeComputer();
	void OnExcludeDomain();

	//
	// UI handlers.
	//

protected:
	//
	// Internal methods.
	//
	CString GenerateReport();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPCMDS_HPP
