/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPHEADERS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	Wrapper to include all the main application headers.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPHEADERS_HPP
#define APPHEADERS_HPP

/******************************************************************************
**
** System headers.
**
*******************************************************************************
*/

#include "wcl.hpp"		// Windows C++ library and core headers.
#include "mdbl.hpp"		// Memory Database Library.
#include "ncl.hpp"		// Network & Comms Library.

/******************************************************************************
**
** Application specific headers.
**
*******************************************************************************
*/

#include "Resource.h"

// App data classes.
#include "Servers.hpp"
#include "Mods.hpp"
#include "GameTypes.hpp"
#include "FavFiles.hpp"
#include "Favourites.hpp"
#include "Summary.hpp"
#include "QueryResponse.hpp"
#include "MasterServer.hpp"
#include "GameServer.hpp"
#include "QueryJob.hpp"
#include "Filter.hpp"
#include "MasterQueryOpts.hpp"
#include "PingOpts.hpp"

// App GUI classes.
#include "AppCmds.hpp"
#include "AppToolBar.hpp"
#include "AppDlg.hpp"
#include "AppWnd.hpp"
#include "UTSvrBrowser.hpp"

#endif //APPHEADERS_HPP
