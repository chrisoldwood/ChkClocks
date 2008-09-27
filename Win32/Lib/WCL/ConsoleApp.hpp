////////////////////////////////////////////////////////////////////////////////
//! \file   ConsoleApp.hpp
//! \brief  The ConsoleApp class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef WCL_CONSOLEAPP_HPP
#define WCL_CONSOLEAPP_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/CmdLineParser.hpp>

namespace WCL
{

////////////////////////////////////////////////////////////////////////////////
//! The base class for Console type applications.

class ConsoleApp
{
public:
	//! Default constructor.
	ConsoleApp();

	//! Destructor.
	virtual ~ConsoleApp();

	//
	// Methods.
	//

	//! The application C++ entry point.
	virtual int Main(int nArgc, tchar* apszArgv[]);

protected:
	//
	// Members.
	//

	//
	// Internal Methods.
	//

	//! Run the application.
	virtual int Run(int nArgc, tchar* apszArgv[]) = 0;

	//! Display the program options syntax.
	virtual void ShowUsage() = 0;
};

//namespace WCL
}

#endif // WCL_CONSOLEAPP_HPP