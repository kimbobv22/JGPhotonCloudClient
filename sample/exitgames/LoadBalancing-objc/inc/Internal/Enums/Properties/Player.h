/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#import "PhotonPeer.h"

namespace LoadBalancing
{
	// Photon properties, internally set by LoadBalancing (LoadBalancing builtin properties)
	namespace Properties
	{
		namespace Player
		{
			static const nByte PLAYERNAME = 255;
		};
	}
}

#endif