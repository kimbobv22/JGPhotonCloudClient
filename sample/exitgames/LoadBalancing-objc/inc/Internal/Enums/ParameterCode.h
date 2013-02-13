/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __PARAMETER_CODE_H
#define __PARAMETER_CODE_H

#import "PhotonPeer.h"

namespace LoadBalancing
{
	namespace ParameterCode
	{
		static const nByte ROOM_NAME              = P_GAMEID;
		static const nByte PLAYERNR               = P_ACTORNR;
		static const nByte TARGET_PLAYERNR        = P_TARGET_ACTORNR;
		static const nByte PLAYER_LIST            = P_ACTOR_LIST;
		static const nByte PROPERTIES             = P_PROPERTIES;
		static const nByte BROADCAST              = P_BROADCAST;
		static const nByte PLAYER_PROPERTIES      = P_ACTOR_PROPERTIES;
		static const nByte ROOM_PROPERTIES        = P_GAME_PROPERTIES;
		static const nByte DATA                   = P_DATA;
		static const nByte CODE                   = P_CODE;
		static const nByte CLEANUP_CACHE_ON_LEAVE = 241;
		static const nByte ADDRESS                = 230;
		static const nByte PEER_COUNT             = 229;
		static const nByte ROOM_COUNT             = 228;
		static const nByte MASTER_PEER_COUNT      = 227;
		static const nByte USER_ID                = 225;
		static const nByte APPLICATION_ID         = 224;
		static const nByte POSITION               = 223;
		static const nByte ROOM_LIST              = 222;
		static const nByte SECRET                 = 221;
		static const nByte APP_VERSION            = 220;
		static const nByte AZURE_NODE_INFO        = 210;
		static const nByte AZURE_LOCAL_NODE_ID    = 209;
		static const nByte AZURE_MASTER_NODE_ID   = 208;
		static const nByte TEST                   = 207;
	};
}

#endif