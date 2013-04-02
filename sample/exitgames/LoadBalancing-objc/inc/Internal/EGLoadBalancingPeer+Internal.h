/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_LOAD_BALANCING_PEER_INTERNAL_H
#define __EG_LOAD_BALANCING_PEER_INTERNAL_H

#import "EGLoadBalancingPeer.h"
#import "OperationCode.h"
#import "ParameterCode.h"
#import "EventCode.h"
#import "JoinType.h"
#import "Player.h"
#import "Room.h"

@interface EGLoadBalancingPeer (Internal)

- (bool) opSetPropertiesOfPlayer:(int)playerNr :(NSDictionary*)properties;
- (bool) opSetPropertiesOfRoom:(NSDictionary*)properties;
+ (NSMutableDictionary*) stripToCustomProperties:(NSDictionary*)properties;
+ (NSMutableDictionary*) stripKeysWithNullValues:(NSDictionary*)orig;
- (NSMutableDictionary*) opCreateRoomImplementation:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(NSDictionary*)customLocalPlayerProperties :(EGArray*)propsListedInLobby;
- (NSMutableDictionary*) opJoinRoomImplementation:(NSString*)gameID :(NSDictionary*)customLocalPlayerProperties;

@end

#endif