/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __LOAD_BALANCING_ROOM_INTERNAL_H
#define __LOAD_BALANCING_ROOM_INTERNAL_H

#import "Room.h"
#import "LoadBalancingRoom.h"
#import "LoadBalancingPlayer+Internal.h"

@interface LoadBalancingRoom (Internal)

- (id) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties;
+ (id) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties;
- (id) cacheProperties:(NSDictionary*)properties;

@end



@interface LoadBalancingMutableRoom (Internal)

- (void) setPlayers:(EGMutableArray*)players;
- (void) setLocalPlayer:(LoadBalancingMutablePlayer*)localPlayer;
- (void) setLocalPlayer:(int)number :(NSDictionary*)properties;

- (id) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties :(LoadBalancingPeer*)peer :(NSArray*)propsListedInLobby;
+ (id) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties :(LoadBalancingPeer*)peer :(NSArray*)propsListedInLobby;

- (void) removeAllPlayers;
- (void) addPlayer:(LoadBalancingPlayer*)player;
- (void) addPlayer:(int)number :(NSDictionary*)properties;
- (bool) removePlayer:(LoadBalancingPlayer*)player;
- (bool) removePlayerWithNumber:(int)number;
- (LoadBalancingPlayer*) createPlayer:(int)number :(NSDictionary*)properties;
- (LoadBalancingMutablePlayer*) createMutablePlayer:(int)number :(NSDictionary*)properties;

@end

#endif