/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __LOAD_BALANCING_ROOM_H
#define __LOAD_BALANCING_ROOM_H

#import "LoadBalancingPlayer.h"

@interface LoadBalancingRoom : Base
{
@protected
	NSString* mName;
	nByte mPlayerCount;
	nByte mMaxPlayers;
	bool mIsOpen;
	NSMutableDictionary* mCustomProperties;
}

@property (readonly, retain) NSString* Name;
@property (readonly) nByte PlayerCount;
@property (readonly) nByte MaxPlayers;
@property (readonly) bool IsOpen;
@property (readonly, copy) NSDictionary* CustomProperties;

- (NSString*) toString:(bool)withTypes :(bool)withCustomProperties;

@end

@interface LoadBalancingMutableRoom : LoadBalancingRoom
{
@private
	LoadBalancingPeer* mLoadBalancingPeer;
	bool mIsVisible;
	EGMutableArray* mPlayers;
	LoadBalancingMutablePlayer* mLocalPlayer;
	int mMasterClientID;
	EGMutableArray* mPropsListedInLobby;
}

@property (readwrite) nByte MaxPlayers;
@property (readwrite) bool IsOpen;
@property (readwrite) bool IsVisible;
@property (readonly) EGArray* Players;
@property (readonly) LoadBalancingMutablePlayer* LocalPlayer;
@property (readonly) int MasterClientID;
@property (readwrite, copy) EGArray* PropsListedInLobby;

- (LoadBalancingPlayer*) getPlayerForNumber:(int)playerNumber;
- (void) mergeCustomProperties:(NSDictionary*)customProperties;
- (void) addCustomProperty:(NSString*)key :(id)value;
- (void) addCustomProperties:(NSDictionary*)customProperties;
- (void) removeCustomProperty:(NSString*)key;
- (void) removeCustomProperties:(NSString**)keys :(NSUInteger)count;
- (NSString*) toString:(bool)withTypes :(bool)withCustomProperties :(bool)withLocalPlayer; // = false
- (NSString*) toString:(bool)withTypes :(bool)withCustomProperties :(bool)withLocalPlayer :(bool)withPlayers;

@end

#endif