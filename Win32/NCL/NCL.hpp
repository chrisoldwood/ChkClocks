/******************************************************************************
**
** MODULE:		NCL.HPP
** COMPONENT:	Network & Comms Library.
** DESCRIPTION:	Wrapper to include all the core library headers.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef NCL_HPP
#define NCL_HPP

/******************************************************************************
**
** Standard headers.
**
*******************************************************************************
*/

#include "wcl.hpp"
#include <ddeml.h>

/******************************************************************************
**
** Library specific headers.
**
*******************************************************************************
*/

// DDE Classes.
#include "DDEInst.hpp"
#include "DDEException.hpp"
#include "DDEString.hpp"
#include "DDEData.hpp"
#include "IDDELinkData.hpp"
#include "DDELink.hpp"
#include "IDDEConvData.hpp"
#include "DDEConv.hpp"
#include "DDECltConv.hpp"
#include "DDESvrConv.hpp"
#include "IDDEClientListener.hpp"
#include "DefDDEClientListener.hpp"
#include "DDEClient.hpp"
#include "IDDEServerListener.hpp"
#include "DefDDEServerListener.hpp"
#include "DDEServer.hpp"

// Named Pipe Classes.
#include "PipeException.hpp"
#include "NamedPipe.hpp"
#include "ClientPipe.hpp"
#include "ServerPipe.hpp"

#endif //NCL_HPP
