/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __JOIN_TYPE_H
#define __JOIN_TYPE_H

#import "PhotonPeer.h"

namespace LoadBalancing
{
	namespace JoinType
	{
		static const nByte CREATE_ROOM         = 255;
		static const nByte JOIN_ROOM           = 254;
		static const nByte JOIN_RANDOM_ROOM    = 253;
		static const nByte AUTO_JOIN_LOBBY     = 252;
		static const nByte EXPLICIT_JOIN_LOBBY = 251;
	};
}

#endif