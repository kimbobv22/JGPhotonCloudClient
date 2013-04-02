/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_LOAD_BALANCING_PLAYER_INTERNAL_H
#define __EG_LOAD_BALANCING_PLAYER_INTERNAL_H

#import "Player.h"
#import "EGLoadBalancingPlayer.h"

@interface EGLoadBalancingPlayer (Internal)

- (id) initLoadBalancingPlayer:(int)number :(NSDictionary*)properties :(const EGLoadBalancingMutableRoom* const)room;
+ (id) loadBalancingPlayer:(int)number :(NSDictionary*)properties :(const EGLoadBalancingMutableRoom* const)room;
- (id) cacheProperties:(NSDictionary*)properties;

@end



@interface EGLoadBalancingMutablePlayer (Internal)

- (id) initLoadBalancingPlayer:(int)number :(NSDictionary*)properties :(const EGLoadBalancingMutableRoom* const)room :(EGLoadBalancingPeer*)peer;
+ (id) loadBalancingPlayer:(int)number :(NSDictionary*)properties :(const EGLoadBalancingMutableRoom* const)room :(EGLoadBalancingPeer*)peer;
- (void) cleanCustomProperties;

@end

#endif