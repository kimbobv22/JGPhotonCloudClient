/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __PHOTON_BASE_LISTENER_H
#define __PHOTON_BASE_LISTENER_H

#include "Photon.h"
#include "Common.h"

namespace ExitGames
{
	namespace Photon
	{
		class PhotonBaseListener
		{
		public:
			virtual ~PhotonBaseListener(void){}
			/**
			   called by the library as callback for debug messages.
			   @details
			   This function is used to relay debug information from the
			   Photon library to your application. It is strongly
			   recommended to implement this callback function, so that in
			   case of errors you can get information about what went wrong.
			   The debug information is divided into different levels of
			   importance, so that you can filter out information when not
			   needed. Use PhotonPeer::setDebugOutputLevel()
			   to set the desired amount of debug output. The default
			   setting displays warnings and errors.
		   
			   All debug output is only available in the DEBUG version of
			   the Photon Library and gets optimized away in the release build.
			   @param debugLevel the debug level, the message was created with
			   @param string a string containing debug information                                                   */
			virtual void debugReturn(PhotonPeer_DebugLevel debugLevel, const Common::JString& string) = 0;
		};

		/** @file */
	}
}

#endif