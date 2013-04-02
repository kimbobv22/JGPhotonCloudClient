//
//  JGPhotonCloudType.h
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 10..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#ifndef ExitgamesCloudTest_JGPhotonCloudType_h
#define ExitgamesCloudTest_JGPhotonCloudType_h

#define JGPhotonCloudClientVersion 0x00000100 //0.1.0

#import "LoadBalancingClient.h"
#import "LoadBalancingPeer.h"
#import "LoadBalancingRoom.h"
#import "LoadBalancingPlayer.h"

namespace JGPhotonCloudPeerState{
	enum State{
		Uninitialized = LoadBalancing::PeerStates::Uninitialized,
		PeerCreated = LoadBalancing::PeerStates::PeerCreated,
		Connecting = LoadBalancing::PeerStates::Connecting,
		Connected = LoadBalancing::PeerStates::Connected,
		Queued = LoadBalancing::PeerStates::Queued,
		Authenticated = LoadBalancing::PeerStates::Authenticated,
		JoinedLobby = LoadBalancing::PeerStates::JoinedLobby,
		DisconnectingFromMasterserver = LoadBalancing::PeerStates::DisconnectingFromMasterserver,
		ConnectingToGameserver = LoadBalancing::PeerStates::ConnectingToGameserver,
		ConnectedToGameserver = LoadBalancing::PeerStates::ConnectedToGameserver,
		AuthenticatedOnGameServer = LoadBalancing::PeerStates::AuthenticatedOnGameServer,
		Joining = LoadBalancing::PeerStates::Joining,
		Joined = LoadBalancing::PeerStates::Joined,
		Leaving = LoadBalancing::PeerStates::Leaving,
		Left = LoadBalancing::PeerStates::Left,
		DisconnectingFromGameserver = LoadBalancing::PeerStates::DisconnectingFromGameserver,
		ConnectingToMasterserver = LoadBalancing::PeerStates::ConnectingToMasterserver,
		ConnectedComingFromGameserver = LoadBalancing::PeerStates::ConnectedComingFromGameserver,
		QueuedComingFromGameserver = LoadBalancing::PeerStates::QueuedComingFromGameserver,
		AuthenticatedComingFromGameserver = LoadBalancing::PeerStates::AuthenticatedComingFromGameserver,
		Disconnecting = LoadBalancing::PeerStates::Disconnecting,
		Disconnected = LoadBalancing::PeerStates::Disconnected,
	};
}

namespace JGPhotonCloudErrorType {
	enum Type{
		ClientError,
		ServerError,
		Warning,
	};
}

#endif
