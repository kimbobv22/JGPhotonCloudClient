/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_LOAD_BALANCING_ROOM_INTERNAL_H
#define __EG_LOAD_BALANCING_ROOM_INTERNAL_H

#import "Room.h"
#import "EGLoadBalancingRoom.h"
#import "EGLoadBalancingPlayer+Internal.h"

@interface EGLoadBalancingRoom (Internal)

- (id) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties;
+ (id) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties;
- (id) cacheProperties:(NSDictionary*)properties;

@end



@interface EGLoadBalancingMutableRoom (Internal)

- (void) setPlayers:(EGMutableArray*)players;
- (void) setLocalPlayer:(EGLoadBalancingMutablePlayer*)localPlayer;
- (void) setLocalPlayer:(int)number :(NSDictionary*)properties;

- (id) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties :(EGLoadBalancingPeer*)peer :(NSArray*)propsListedInLobby;
+ (id) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties :(EGLoadBalancingPeer*)peer :(NSArray*)propsListedInLobby;

- (void) removeAllPlayers;
- (void) addPlayer:(EGLoadBalancingPlayer*)player;
- (void) addPlayer:(int)number :(NSDictionary*)properties;
- (bool) removePlayer:(EGLoadBalancingPlayer*)player;
- (bool) removePlayerWithNumber:(int)number;
- (EGLoadBalancingPlayer*) createPlayer:(int)number :(NSDictionary*)properties;
- (EGLoadBalancingMutablePlayer*) createMutablePlayer:(int)number :(NSDictionary*)properties;

@end

#endif