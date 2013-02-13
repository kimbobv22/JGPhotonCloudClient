/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __PEER_STATES_H
#define __PEER_STATES_H

namespace LoadBalancing
{
	namespace PeerStates
	{
		enum PeerStates
		{
			Uninitialized,
			PeerCreated,
			Connecting,
			Connected,
			Queued,
			Authenticated,
			JoinedLobby,
			DisconnectingFromMasterserver,
			ConnectingToGameserver,
			ConnectedToGameserver,
			AuthenticatedOnGameServer,
			Joining,
			Joined,
			Leaving,
			Left,
			DisconnectingFromGameserver,
			ConnectingToMasterserver,
			ConnectedComingFromGameserver,
			QueuedComingFromGameserver,
			AuthenticatedComingFromGameserver,
			Disconnecting,
			Disconnected,
		};
		/** @file */
	}
}

#endif