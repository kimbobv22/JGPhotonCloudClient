/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __LOAD_BALANCING_PEER_H
#define __LOAD_BALANCING_PEER_H

#import "ErrorCode.h"
#import "PeerStates.h"
#import "PhotonPeer.h"

@interface LoadBalancingPeer : PhotonPeer
{
}

- (bool) opJoinLobby;
- (bool) opLeaveLobby;
- (bool) opCreateRoom:(NSString*)gameID;
- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen;
- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers;
- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties;
- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(NSDictionary*)customLocalPlayerProperties;
- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(NSDictionary*)customLocalPlayerProperties :(NSArray*)propsListedInLobby;
- (bool) opJoinRoom:(NSString*)gameID;
- (bool) opJoinRoom:(NSString*)gameID :(NSDictionary*)customLocalPlayerProperties;
- (bool) opJoinRandomRoom;
- (bool) opJoinRandomRoom:(NSDictionary*)customRoomProperties;
- (bool) opJoinRandomRoom:(NSDictionary*)customRoomProperties :(nByte)maxPlayers;
- (bool) opLeaveRoom;
- (bool) opRaiseEvent:(bool)reliable :(NSDictionary*)parameters :(nByte)eventCode;
- (bool) opRaiseEvent:(bool)reliable :(NSDictionary*)parameters :(nByte)eventCode :(nByte)channelID;
- (bool) opRaiseEvent:(bool)reliable :(NSDictionary*)parameters :(nByte)eventCode :(nByte)channelID :(int*)targetPlayers :(short)numTargetPlayers;
- (bool) opRaiseEventCached:(bool)sendReliable :(NSDictionary*)evData :(nByte)eventCode :(nByte)channelID :(nByte)eventCaching;
- (bool) opRaiseEventCached:(bool)sendReliable :(NSDictionary*)evData :(nByte)eventCode :(nByte)channelID :(nByte)eventCaching :(nByte)receiverGroup;
- (bool) opAuthenticate:(NSString*)appID :(NSString*)appVersion :(bool)encrypted;

@end

#endif