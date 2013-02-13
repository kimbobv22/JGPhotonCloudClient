/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2012 by Exit Games. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __OPERATION_CODE_H
#define __OPERATION_CODE_H

#include "PhotonPeer.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		namespace OperationCode
		{
			static const nByte LEAVE            = OPC_RT_LEAVE;
			static const nByte RAISE_EVENT      = OPC_RT_RAISE_EV;
			static const nByte SET_PROPERTIES   = OPC_RT_SETPROPERTIES;
			static const nByte GET_PROPERTIES   = OPC_RT_GETPROPERTIES;
			static const nByte AUTHENTICATE     = 230;
			static const nByte JOIN_LOBBY       = 229;
			static const nByte LEAVE_LOBBY      = 228;
			static const nByte CREATE_ROOM      = 227;
			static const nByte JOIN_ROOM        = 226;
			static const nByte JOIN_RANDOM_ROOM = 225;
		};
	}
}

#endif