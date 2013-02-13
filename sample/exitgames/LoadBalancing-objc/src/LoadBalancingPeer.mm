/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2012 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#import "LoadBalancingPeer+Internal.h"

/** @file LoadBalancingPeer.h */

@interface LoadBalancingPeer (Private)

- (bool) opRaiseEventImplementation:(bool)reliable :(NSDictionary*)parameters :(nByte)eventCode :(nByte)channelID :(int*)targetPlayers :(short)numTargetPlayers :(nByte)eventCaching :(nByte)receiverGroup;

@end

using namespace LoadBalancing;

@implementation LoadBalancingPeer

- (bool) opJoinLobby
{
	return [super opCustom:[OperationRequest operationRequestWithOperationCode:OperationCode::JOIN_LOBBY] :true];
}

- (bool) opLeaveLobby
{
	return [super opCustom:[OperationRequest operationRequestWithOperationCode:OperationCode::LEAVE_LOBBY] :true];
}

- (bool) opCreateRoom:(NSString*)gameID
{
	return [self opCreateRoom:gameID :true :true];
}

- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen
{
	return [self opCreateRoom:gameID :isVisible :isOpen :0 :nil];
}

- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers
{
	return [self opCreateRoom:gameID :isVisible :isOpen :maxPlayers :nil];
}

- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties
{
	return [self opCreateRoom:gameID :isVisible :isOpen :maxPlayers :customRoomProperties :nil];
}

- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(NSDictionary*)customLocalPlayerProperties
{
	return [self opCreateRoom:gameID :isVisible :isOpen :maxPlayers :customRoomProperties :customLocalPlayerProperties :nil];
}

- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(NSDictionary*)customLocalPlayerProperties :(EGArray*)propsListedInLobby;
{
	return [self opCustom:[OperationRequest operationRequestWithOperationCode:OperationCode::CREATE_ROOM :[self opCreateRoomImplementation:gameID :isVisible :isOpen :maxPlayers :customRoomProperties :customLocalPlayerProperties :propsListedInLobby]] :true];
}

- (bool) opJoinRoom:(NSString*)gameID
{
	return [self opJoinRoom:gameID :nil];
}

- (bool) opJoinRoom:(NSString*)gameID :(NSDictionary*)customLocalPlayerProperties
{
	return gameID?[super opCustom:[OperationRequest operationRequestWithOperationCode:OperationCode::JOIN_ROOM :[self opJoinRoomImplementation:gameID :customLocalPlayerProperties]] :true]:false;
}

- (bool) opJoinRandomRoom
{
	return [self opJoinRandomRoom:nil];
}

- (bool) opJoinRandomRoom:(NSDictionary*)customRoomProperties
{
	return [self opJoinRandomRoom:customRoomProperties :nil];
}

- (bool) opJoinRandomRoom:(NSDictionary*)customRoomProperties :(nByte)maxPlayers
{
	NSMutableDictionary* roomProps = [LoadBalancingPeer stripToCustomProperties:customRoomProperties];
	if(maxPlayers)
		[roomProps setObject:[NSValue value:&maxPlayers withObjCType:@encode(nByte)] forKey:[NSValue value:&Properties::Room::MAX_PLAYERS withObjCType:@encode(nByte)]];
	return [super opCustom:[OperationRequest operationRequestWithOperationCode:OperationCode::JOIN_RANDOM_ROOM :roomProps.count?[NSDictionary dictionaryWithObject:roomProps forKey:[NSValue value:&ParameterCode::ROOM_PROPERTIES withObjCType:@encode(nByte)]]:nil] :true];
}

- (bool) opLeaveRoom
{
	return [super opCustom:[OperationRequest operationRequestWithOperationCode:LoadBalancing::OperationCode::LEAVE] :true];
}

- (bool) opRaiseEvent:(bool)reliable :(NSDictionary*)parameters :(nByte)eventCode
{
	return [self opRaiseEvent:reliable :parameters :eventCode :0];
}

- (bool) opRaiseEvent:(bool)reliable :(NSDictionary*)parameters :(nByte)eventCode :(nByte)channelID
{
	return [self opRaiseEvent:reliable :parameters :eventCode :channelID :NULL :0];
}

- (bool) opRaiseEvent:(bool)reliable :(NSDictionary*)parameters :(nByte)eventCode :(nByte)channelID :(int*)targetPlayers :(short)numTargetPlayers
{
	return [self opRaiseEventImplementation:reliable :parameters :eventCode :channelID :targetPlayers :numTargetPlayers :EVC_DO_NOT_CACHE :RG_OTHERS];
}

- (bool) opRaiseEventCached:(bool)reliable :(NSDictionary*)evData :(nByte)eventCode :(nByte)channelID :(nByte)eventCaching
{
	return [self opRaiseEventCached:reliable :evData :eventCode :channelID :eventCaching :RG_OTHERS];
}

- (bool) opRaiseEventCached:(bool)reliable :(NSDictionary*)evData :(nByte)eventCode :(nByte)channelID :(nByte)eventCaching :(nByte)receiverGroup
{
	return [self opRaiseEventImplementation:reliable :evData :eventCode :channelID :NULL :0 :eventCaching :receiverGroup];
}

- (bool) opAuthenticate:(NSString*)appID :(NSString*)appVersion :(bool)encrypted
{
	NSMutableDictionary* op = [NSMutableDictionary dictionary];
	[op setObject:appID forKey:[NSValue value:&ParameterCode::APPLICATION_ID withObjCType:@encode(nByte)]];
	[op setObject:appVersion forKey:[NSValue value:&ParameterCode::APP_VERSION withObjCType:@encode(nByte)]];
	PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_INFO, [[OperationRequest operationRequestWithOperationCode:OperationCode::AUTHENTICATE :op] toString:true]);
	return [super opCustom:[OperationRequest operationRequestWithOperationCode:OperationCode::AUTHENTICATE :op] :true :0 :encrypted];
}



// inherited

- (void) dealloc
{
	[super dealloc];
}

@end



@implementation LoadBalancingPeer (Internal)

- (bool) opSetPropertiesOfPlayer:(int)playerNr :(NSDictionary*)properties
{
	NSMutableDictionary* op = [NSMutableDictionary dictionary];
	[op setObject:properties forKey:[NSValue value:&P_PROPERTIES withObjCType:@encode(nByte)]];
	[op setObject:[NSValue value:&playerNr withObjCType:@encode(typeof(playerNr))] forKey:[NSValue value:&P_ACTORNR withObjCType:@encode(nByte)]];
	bool broadcast = true;
	[op setObject:[NSValue value:&broadcast withObjCType:@encode(typeof(broadcast))] forKey:[NSValue value:&P_BROADCAST withObjCType:@encode(nByte)]];

	return [super opCustom:[OperationRequest operationRequestWithOperationCode:OperationCode::SET_PROPERTIES :op] :true];
}

- (bool) opSetPropertiesOfRoom:(NSDictionary*)properties
{
	NSMutableDictionary* op = [NSMutableDictionary dictionary];
	[op setObject:properties forKey:[NSValue value:&P_PROPERTIES withObjCType:@encode(nByte)]];
	bool broadcast = true;
	[op setObject:[NSValue value:&broadcast withObjCType:@encode(typeof(broadcast))] forKey:[NSValue value:&P_BROADCAST withObjCType:@encode(nByte)]];
	
	return [super opCustom:[OperationRequest operationRequestWithOperationCode:OperationCode::SET_PROPERTIES :op] :true];
}

+ (NSMutableDictionary*) stripToCustomProperties:(NSDictionary*)properties
{
	NSArray* keys = properties&&properties.count?[NSArray arrayWithArray:properties.allKeys]:nil;
	NSMutableDictionary* retVal = [NSMutableDictionary dictionary];
	for(NSUInteger i=0; i<properties.count; i++)
	{
		id key = [keys objectAtIndex:i];
		if([key isKindOfClass:[NSString class]])
		{
			id val = [properties objectForKey:key];
			[retVal setObject:[[val respondsToSelector:@selector(copyDeep)]?[val copyDeep]:[val copy] autorelease] forKey:key];
		}
	}
	return retVal;
}

+ (NSMutableDictionary*) stripKeysWithNullValues:(NSDictionary*)orig
{
	NSMutableDictionary* stripped = [[orig copyDeep] autorelease];
	NSArray* keys = orig.allKeys;
	for(NSUInteger i=0; i<keys.count; i++)
		if([[stripped objectForKey:[keys objectAtIndex:i]] isEqual:NSNull.null])
			[stripped removeObjectForKey:[keys objectAtIndex:i]];
	return stripped;
}

- (NSMutableDictionary*) opCreateRoomImplementation:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(NSDictionary*)customLocalPlayerProperties :(EGArray*)propsListedInLobby
{
	NSMutableDictionary* op = [NSMutableDictionary dictionary];
	if(gameID)
		[op setObject:gameID forKey:[NSValue value:&ParameterCode::ROOM_NAME withObjCType:@encode(nByte)]];
	NSMutableDictionary* roomProps = [LoadBalancingPeer stripToCustomProperties:customRoomProperties];
	if(!isOpen)
		[roomProps setObject:[NSValue value:&isOpen withObjCType:@encode(bool)] forKey:[NSValue value:&Properties::Room::IS_OPEN withObjCType:@encode(nByte)]];
	if(!isVisible)
		[roomProps setObject:[NSValue value:&isVisible withObjCType:@encode(bool)] forKey:[NSValue value:&Properties::Room::IS_VISIBLE withObjCType:@encode(nByte)]];
	if(maxPlayers)
		[roomProps setObject:[NSValue value:&maxPlayers withObjCType:@encode(nByte)] forKey:[NSValue value:&Properties::Room::MAX_PLAYERS withObjCType:@encode(nByte)]];
	[roomProps setObject:[propsListedInLobby.Type isEqual:NSStringFromClass([NSString class])]?[[propsListedInLobby copyDeep] autorelease]:[EGArray arrayWithType:NSStringFromClass([NSString class])] forKey:[NSValue value:&Properties::Room::PROPS_LISTED_IN_LOBBY withObjCType:@encode(nByte)]];

	[op setObject:roomProps forKey:[NSValue value:&ParameterCode::ROOM_PROPERTIES withObjCType:@encode(nByte)]];
	NSMutableDictionary* playerProps = [LoadBalancingPeer stripToCustomProperties:customLocalPlayerProperties];
	if(playerProps.count)
		[op setObject:playerProps forKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]];
	bool broadcast = true;
	[op setObject:[NSValue value:&broadcast withObjCType:@encode(bool)] forKey:[NSValue value:&ParameterCode::BROADCAST withObjCType:@encode(nByte)]];
	bool cleanupCacheOnLeave = true;
	[op setObject:[NSValue value:&cleanupCacheOnLeave withObjCType:@encode(bool)] forKey:[NSValue value:&ParameterCode::CLEANUP_CACHE_ON_LEAVE withObjCType:@encode(nByte)]];
	return op;
}

- (NSMutableDictionary*) opJoinRoomImplementation:(NSString*)gameID :(NSDictionary*)customLocalPlayerProperties
{
	NSMutableDictionary* op = [NSMutableDictionary dictionary];
	[op setObject:gameID forKey:[NSValue value:&ParameterCode::ROOM_NAME withObjCType:@encode(nByte)]];
	NSMutableDictionary* playerProps = [LoadBalancingPeer stripToCustomProperties:customLocalPlayerProperties];
	if(playerProps)
		[op setObject:playerProps forKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]];
	bool broadcast = true;
	[op setObject:[NSValue value:&broadcast withObjCType:@encode(bool)] forKey:[NSValue value:&ParameterCode::BROADCAST withObjCType:@encode(nByte)]];
	return op;
}

@end



@implementation LoadBalancingPeer (Private)

- (bool) opRaiseEventImplementation:(bool)reliable :(NSDictionary*)parameters :(nByte)eventCode :(nByte)channelID :(int*)targetPlayers :(short)numTargetPlayers :(nByte)eventCaching :(nByte)receiverGroup
{
	NSMutableDictionary* op = [NSMutableDictionary dictionary];
	[op setObject:parameters forKey:[NSValue value:&P_DATA withObjCType:@encode(nByte)]];
	[op setObject:[NSValue value:&eventCode withObjCType:@encode(typeof(eventCode))] forKey:[NSValue value:&P_CODE withObjCType:@encode(nByte)]];
	if(targetPlayers)
	{
		EGMutableArray* array = [EGMutableArray arrayWithCapacityAndCType:numTargetPlayers :@encode(typeof(*targetPlayers))];
		for(short i=0; i<numTargetPlayers; i++)
			[array addObject:[NSValue value:targetPlayers+i withObjCType:@encode(typeof(*targetPlayers))]];
		[op setObject:array forKey:[NSValue value:&P_ACTOR_LIST withObjCType:@encode(nByte)]];
	}
	else
	{
		if(eventCaching != EVC_DO_NOT_CACHE)
			[op setObject:[NSValue value:&eventCaching withObjCType:@encode(typeof(eventCaching))]forKey:[NSValue value:&P_CACHE withObjCType:@encode(nByte)]];
		if(receiverGroup != RG_OTHERS)
			[op setObject:[NSValue value:&receiverGroup withObjCType:@encode(typeof(receiverGroup))]forKey:[NSValue value:&P_RECEIVER_GROUP withObjCType:@encode(nByte)]];
	}

	return [super opCustom:[OperationRequest operationRequestWithOperationCode:OperationCode::RAISE_EVENT :op] :reliable :channelID];
}

@end