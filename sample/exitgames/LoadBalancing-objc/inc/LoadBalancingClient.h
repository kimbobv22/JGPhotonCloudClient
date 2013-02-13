/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __LOAD_BALANCING_CLIENT_H
#define __LOAD_BALANCING_CLIENT_H

#import "LoadBalancingRoom.h"
#import "LoadBalancingListener.h"

@interface LoadBalancingClient : LoadBalancingPeer <PhotonListener>
{
@private
	id<LoadBalancingListener> mListener;
	NSString* mGameserver;
	NSString* mAppVersion;
	NSString* mRoomName;
	NSString* mAppID;
	int mPeerCount;
	int mRoomCount;
	int mMasterPeerCount;
	nByte mLastJoinType;
	int mQueuePosition;
	LoadBalancingMutableRoom* mCurrentlyJoinedRoom;
	NSMutableDictionary* mRoomList;
	int mCachedErrorCodeFromGameServer;
	const NSString* mCachedErrorStringFromGameServer;
	bool mUseTcp;
	NSString* mMasterserver;
	bool mAutoJoinLobby;

	enum LoadBalancing::PeerStates::PeerStates mState;
}

@property (readonly) LoadBalancing::PeerStates::PeerStates State;
@property (readonly) const NSString* MasterserverAddress;
@property (readonly) int CountPlayersIngame;
@property (readonly) int CountGamesRunning;
@property (readonly) int CountPlayersOnline;
@property (readonly) int QueuePosition;
@property (readonly) LoadBalancingMutableRoom* CurrentlyJoinedRoom;
@property (readonly) NSDictionary* RoomList;
@property (readonly) bool IsInRoom;
@property (readonly) bool IsInGameRoom;
@property (readonly) bool IsInLobby;
@property (readwrite) bool AutoJoinLobby;

- (id) initClient:(id<LoadBalancingListener>)listener :(NSString*)applicationID :(NSString*)appVersion :(NSString*)username;
- (id) initClient:(id<LoadBalancingListener>)listener :(NSString*)applicationID :(NSString*)appVersion :(NSString*)username :(bool)useTcp;
- (bool) connect;
- (bool) opJoinRandomRoom:(NSDictionary*)customRoomProperties :(nByte)maxPlayers :(NSDictionary*)customLocalPlayerProperties;

@end

#endif