//
//  JGPhotonCloudType.h
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 10..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#ifndef ExitgamesCloudTest_JGPhotonCloudType_h
#define ExitgamesCloudTest_JGPhotonCloudType_h

#define JGPhotonCloudClientVersion 0x00000300 //0.3.0

#import "EGLoadBalancingClient.h"
#import "EGLoadBalancingPeer.h"
#import "EGLoadBalancingRoom.h"
#import "EGLoadBalancingPlayer.h"

namespace JGPhotonCloudPeerState{
	enum State{
		Uninitialized = ExitGames::LoadBalancing::PeerStates::Uninitialized,
		PeerCreated = ExitGames::LoadBalancing::PeerStates::PeerCreated,
		Connecting = ExitGames::LoadBalancing::PeerStates::Connecting,
		Connected = ExitGames::LoadBalancing::PeerStates::Connected,
		Queued = ExitGames::LoadBalancing::PeerStates::Queued,
		Authenticated = ExitGames::LoadBalancing::PeerStates::Authenticated,
		JoinedLobby = ExitGames::LoadBalancing::PeerStates::JoinedLobby,
		DisconnectingFromMasterserver = ExitGames::LoadBalancing::PeerStates::DisconnectingFromMasterserver,
		ConnectingToGameserver = ExitGames::LoadBalancing::PeerStates::ConnectingToGameserver,
		ConnectedToGameserver = ExitGames::LoadBalancing::PeerStates::ConnectedToGameserver,
		AuthenticatedOnGameServer = ExitGames::LoadBalancing::PeerStates::AuthenticatedOnGameServer,
		Joining = ExitGames::LoadBalancing::PeerStates::Joining,
		Joined = ExitGames::LoadBalancing::PeerStates::Joined,
		Leaving = ExitGames::LoadBalancing::PeerStates::Leaving,
		Left = ExitGames::LoadBalancing::PeerStates::Left,
		DisconnectingFromGameserver = ExitGames::LoadBalancing::PeerStates::DisconnectingFromGameserver,
		ConnectingToMasterserver = ExitGames::LoadBalancing::PeerStates::ConnectingToMasterserver,
		ConnectedComingFromGameserver = ExitGames::LoadBalancing::PeerStates::ConnectedComingFromGameserver,
		QueuedComingFromGameserver = ExitGames::LoadBalancing::PeerStates::QueuedComingFromGameserver,
		AuthenticatedComingFromGameserver = ExitGames::LoadBalancing::PeerStates::AuthenticatedComingFromGameserver,
		Disconnecting = ExitGames::LoadBalancing::PeerStates::Disconnecting,
		Disconnected = ExitGames::LoadBalancing::PeerStates::Disconnected,
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
