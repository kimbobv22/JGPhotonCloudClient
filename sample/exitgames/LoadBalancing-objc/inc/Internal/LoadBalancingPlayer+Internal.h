/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __LOAD_BALANCING_PLAYER_INTERNAL_H
#define __LOAD_BALANCING_PLAYER_INTERNAL_H

#import "Player.h"
#import "LoadBalancingPlayer.h"

@interface LoadBalancingPlayer (Internal)

- (id) initLoadBalancingPlayer:(int)number :(NSDictionary*)properties :(const LoadBalancingMutableRoom* const)room;
+ (id) loadBalancingPlayer:(int)number :(NSDictionary*)properties :(const LoadBalancingMutableRoom* const)room;
- (id) cacheProperties:(NSDictionary*)properties;

@end



@interface LoadBalancingMutablePlayer (Internal)

- (id) initLoadBalancingPlayer:(int)number :(NSDictionary*)properties :(const LoadBalancingMutableRoom* const)room :(LoadBalancingPeer*)peer;
+ (id) loadBalancingPlayer:(int)number :(NSDictionary*)properties :(const LoadBalancingMutableRoom* const)room :(LoadBalancingPeer*)peer;
- (void) cleanCustomProperties;

@end

#endif