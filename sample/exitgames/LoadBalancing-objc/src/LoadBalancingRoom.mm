/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2012 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#import "LoadBalancingPeer+Internal.h"
#import "LoadBalancingRoom+Internal.h"
#import "Internal/Enums/Properties/Room.h"

/** @file LoadBalancingRoom.h */

using namespace LoadBalancing;

@interface LoadBalancingRoom (Private)

- (NSString*) payloadToString:(bool)withTypes :(bool)withCustomProperties;

@end



@implementation LoadBalancingRoom

@synthesize Name = mName;
@synthesize PlayerCount = mPlayerCount;
@synthesize MaxPlayers = mMaxPlayers;
@synthesize IsOpen = mIsOpen;

- (NSDictionary*) CustomProperties
{
	return [[mCustomProperties copyDeep] autorelease];
}



- (id) init
{
	NSAssert(false, @"not implemented");
	return nil;
}

- (NSString*) toString:(bool)withTypes
{
	return [self toString:withTypes :false];
}

- (NSString*) toString:(bool)withTypes :(bool)withCustomProperties
{
	return [NSString stringWithFormat:@"%@={%@}", mName, [self payloadToString:withTypes :withCustomProperties]];
}


@end



@implementation LoadBalancingRoom (Internal)

- (id) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties
{
	self = [super init];
	mName = [name retain];
	mCustomProperties = [[NSMutableDictionary alloc] init];
	return [self cacheProperties:properties];
}

+ (id) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties
{
	return [[[self alloc] initLoadBalancingRoom:name :properties] autorelease];
}

- (id) cacheProperties:(NSDictionary*)properties
{
	if([properties objectForKey:[NSValue value:&Properties::Room::MAX_PLAYERS withObjCType:@encode(nByte)]]) [[properties objectForKey:[NSValue value:&Properties::Room::MAX_PLAYERS withObjCType:@encode(nByte)]] getValue:&mMaxPlayers];
	if([properties objectForKey:[NSValue value:&Properties::Room::IS_OPEN withObjCType:@encode(nByte)]]) [[properties objectForKey:[NSValue value:&Properties::Room::IS_OPEN withObjCType:@encode(nByte)]] getValue:&mIsOpen];
	if([properties objectForKey:[NSValue value:&Properties::Room::PLAYER_COUNT withObjCType:@encode(nByte)]]) [[properties objectForKey:[NSValue value:&Properties::Room::PLAYER_COUNT withObjCType:@encode(nByte)]] getValue:&mPlayerCount];
	[mCustomProperties addEntriesFromDictionary:[LoadBalancingPeer stripToCustomProperties:properties]];
	mCustomProperties = [[LoadBalancingPeer stripKeysWithNullValues:[mCustomProperties autorelease]] retain];
	return self;
}

- (BOOL) isEqual:(id)object
{
	return [object isKindOfClass:[LoadBalancingRoom class]] && [self.Name compare:((LoadBalancingRoom*)object).Name] == NSOrderedSame;
}

- (NSUInteger)hash
{
	return [self.Name hash];
}

- (void) dealloc
{
	[mCustomProperties release];
	[mName release];
	[super dealloc];
}

@end



@implementation LoadBalancingRoom (Private)

- (NSString*) payloadToString:(bool)withTypes :(bool)withCustomProperties
{
	NSString* res = [NSString stringWithFormat:@"pl: %d, max: %d, open: %@", mPlayerCount, mMaxPlayers, mIsOpen?@"true":@"false"];
	if(withCustomProperties && mCustomProperties.count)
		res = [res stringByAppendingFormat:@", props: %@", [mCustomProperties toString:withTypes]];
	return res;
}

@end



@interface LoadBalancingMutableRoom (Private)

- (NSString*) payloadToString:(bool)withTypes :(bool)withCustomProperties :(bool)withLocalPlayer :(bool)withPlayers;

@end



@implementation LoadBalancingMutableRoom

- (nByte) MaxPlayers
{
	return super.MaxPlayers;
}

- (void) setMaxPlayers:(nByte)maxPlayers
{
	[mLoadBalancingPeer opSetPropertiesOfRoom:[NSDictionary dictionaryWithObject:[NSValue value:&(mMaxPlayers=maxPlayers) withObjCType:@encode(nByte)] forKey:[NSValue value:&Properties::Room::MAX_PLAYERS withObjCType:@encode(nByte)]]];
}

- (bool) IsOpen
{
	return super.IsOpen;
}

- (void) setIsOpen:(bool)isOpen
{
	[mLoadBalancingPeer opSetPropertiesOfRoom:[NSDictionary dictionaryWithObject:[NSValue value:&(mIsOpen=isOpen) withObjCType:@encode(bool)] forKey:[NSValue value:&Properties::Room::IS_OPEN withObjCType:@encode(nByte)]]];
}

- (bool) IsVisible
{
	return mIsVisible;
}

- (void) setIsVisible:(bool)isVisible
{
	[mLoadBalancingPeer opSetPropertiesOfRoom:[NSDictionary dictionaryWithObject:[NSValue value:&(mIsVisible=isVisible) withObjCType:@encode(bool)] forKey:[NSValue value:&Properties::Room::IS_VISIBLE withObjCType:@encode(nByte)]]];
}

@synthesize Players = mPlayers;
@synthesize LocalPlayer = mLocalPlayer;
@synthesize MasterClientID = mMasterClientID;

- (EGArray*) PropsListedInLobby
{
	return [[mPropsListedInLobby copyDeep] autorelease];
}

- (void) setPropsListedInLobby:(EGArray*)propsListedInLobby
{
	if(mPropsListedInLobby != propsListedInLobby && ![mPropsListedInLobby isEqual:propsListedInLobby])
	{
		[mPropsListedInLobby release];
		[mLoadBalancingPeer opSetPropertiesOfRoom:[NSDictionary dictionaryWithObject:mPropsListedInLobby=[propsListedInLobby.Type isEqual:NSStringFromClass([NSString class])]?[[propsListedInLobby copyDeep] autorelease]:[EGArray arrayWithType:NSStringFromClass([NSString class])] forKey:[NSValue value:&Properties::Room::PROPS_LISTED_IN_LOBBY withObjCType:@encode(nByte)]]];
	}
}



- (LoadBalancingPlayer*) getPlayerForNumber:(int)playerNumber
{
	for(unsigned i=0; i<mPlayers.count; i++)
		if(((LoadBalancingPlayer*)[mPlayers objectAtIndex:i]).Number == playerNumber)
			return [mPlayers objectAtIndex:i];
	return nil;
}

- (void) mergeCustomProperties:(NSDictionary*)customProperties
{
	if(mCustomProperties == customProperties)
		return;
	NSDictionary* stripDict = [LoadBalancingPeer stripToCustomProperties:customProperties];
	if(!stripDict.count)
		return;
	NSDictionary* oldDict = [[mCustomProperties copyDeep] autorelease];
	[mCustomProperties addEntriesFromDictionary:stripDict];
	mCustomProperties = [[LoadBalancingPeer stripKeysWithNullValues:[mCustomProperties autorelease]] retain];
	if(![mCustomProperties isEqual:oldDict])
		[mLoadBalancingPeer opSetPropertiesOfRoom:stripDict];	
}

- (void) addCustomProperty:(NSString*)key :(id)value
{
	[self addCustomProperties:[NSDictionary dictionaryWithObject:value forKey:key]];
}

- (void) addCustomProperties:(NSDictionary*)customProperties
{
	[self mergeCustomProperties:[LoadBalancingPeer stripKeysWithNullValues:customProperties]];
}

- (void) removeCustomProperty:(NSString*)key
{
	[self removeCustomProperties:&key :1];
}

- (void) removeCustomProperties:(NSString**)keys :(NSUInteger)count
{
	NSNull** vals = new NSNull*[count];
	for(NSUInteger i=0; i<count; i++)
		vals[i] = [NSNull null];
	[self mergeCustomProperties:[NSDictionary dictionaryWithObjects:vals forKeys:keys count:count]];
	delete vals;
}

- (NSString*) toString:(bool)withTypes :(bool)withCustomProperties
{
	return [self toString:withTypes :withCustomProperties : false];
}

- (NSString*) toString:(bool)withTypes :(bool)withCustomProperties :(bool)withLocalPlayer
{
	return [self toString:withTypes :withCustomProperties :withLocalPlayer : false];
}

- (NSString*) toString:(bool)withTypes :(bool)withCustomProperties :(bool)withLocalPlayer :(bool)withPlayers
{
	return [NSString stringWithFormat:@"%@={%@}", mName, [self payloadToString:withTypes :withCustomProperties :withLocalPlayer :withPlayers]];
}

@end



@implementation LoadBalancingMutableRoom (Internal)

- (void) setPlayers:(EGMutableArray*)players
{
	if(mPlayers == players)
		return;
	[mPlayers release];
	mPlayers = [players retain];
}

- (void) setLocalPlayer:(LoadBalancingMutablePlayer*)localPlayer
{
	if(mLocalPlayer == localPlayer)
		return;
	[mLocalPlayer release];
	mLocalPlayer = [localPlayer retain];
}

- (void) setLocalPlayer:(int)number :(NSDictionary*)properties
{
	self.LocalPlayer = [self createMutablePlayer:number :properties];
}



- (id) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties
{
	NSAssert(false, @"not implemented");
	return nil;
}

- (id) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties :(LoadBalancingPeer*)peer :(NSArray*)propsListedInLobby
{
	self = [super initLoadBalancingRoom:name :properties];
	mLoadBalancingPeer = [peer retain];
	mPlayers = [[EGMutableArray alloc] initWithType:NSStringFromClass([LoadBalancingPlayer class])];
	mLocalPlayer = [[LoadBalancingMutablePlayer alloc] initLoadBalancingPlayer:-1 :[NSDictionary dictionary] :self :mLoadBalancingPeer];
	mMasterClientID = 0;
	mPropsListedInLobby = [propsListedInLobby copyDeep];
	return self;
}

+ (id) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties
{
	NSAssert(false, @"not implemented");
	return nil;
}

+ (id) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties :(LoadBalancingPeer*)peer :(NSArray*)propsListedInLobby
{
	return [[[self alloc] initLoadBalancingRoom:name :properties :peer :propsListedInLobby] autorelease];
}

- (id) cacheProperties:(NSDictionary*)properties
{
	if([properties objectForKey:[NSValue value:&Properties::Room::IS_VISIBLE withObjCType:@encode(nByte)]]) [[properties objectForKey:[NSValue value:&Properties::Room::IS_VISIBLE withObjCType:@encode(nByte)]] getValue:&mIsVisible];
	return [super cacheProperties:properties];
}

- (void) removeAllPlayers
{
	[mPlayers removeAllObjects];
}

- (void) addPlayer:(LoadBalancingPlayer*)player
{
	[mPlayers addObject:player];
	if(!mMasterClientID || player.Number < mMasterClientID)
		mMasterClientID = player.Number;
}

- (void) addPlayer:(int)number :(NSDictionary*)properties
{
	[self addPlayer:[self createPlayer:number :properties]];
}

- (bool) removePlayer:(LoadBalancingPlayer*)player
{
	NSUInteger oldCount = mPlayers.count;
	[mPlayers removeObject:player];
	bool retVal = mPlayers.count < oldCount;
	if(player.Number == mMasterClientID)
	{
		mMasterClientID = mPlayers.count?[[mPlayers objectAtIndex:0] Number]:0;
		for(unsigned int i=1; i<mPlayers.count; ++i)
			if([[mPlayers objectAtIndex:i] Number] < mMasterClientID)
				mMasterClientID = [[mPlayers objectAtIndex:i] Number];
	}
	return retVal;
}

- (bool) removePlayerWithNumber:(int)number
{
	return [self removePlayer:[self createPlayer:number :[NSDictionary dictionary]]];
}

- (LoadBalancingPlayer*) createPlayer:(int)number :(NSDictionary*)properties
{
	return [LoadBalancingPlayer loadBalancingPlayer:number :properties :self];
}

- (LoadBalancingMutablePlayer*) createMutablePlayer:(int)number :(NSDictionary*)properties
{
	return [LoadBalancingMutablePlayer loadBalancingPlayer:number :properties :self :mLoadBalancingPeer];
}

- (void) dealloc
{
	[mPropsListedInLobby release];
	[mLocalPlayer release];
	[mPlayers release];
	[mLoadBalancingPeer release];
	[super dealloc];
}

@end



@implementation LoadBalancingMutableRoom (Private)

- (NSString*) payloadToString:(bool)withTypes :(bool)withCustomProperties :(bool)withLocalPlayer :(bool)withPlayers
{
	NSMutableString* res = [NSMutableString stringWithFormat:@"%@, visible: %@, masterClient: %d", [super payloadToString:false :false], mIsVisible?@"true":@"false", mMasterClientID];
	if(mPropsListedInLobby.count)
		[res appendFormat:@", propsListedInLobby: %@", [mPropsListedInLobby toString:withTypes]];
	if(withCustomProperties && mCustomProperties.count)
		[res appendFormat:@", props: %@", [mCustomProperties toString:withTypes]];
	if(withLocalPlayer && mLocalPlayer)
		[res appendFormat:@", localPlayer: %@", [mLocalPlayer toString:withTypes :withCustomProperties]];
	if(withPlayers && mPlayers.count)
	{
		[res appendFormat:@", players:"];
		for(NSUInteger i=0; i<mPlayers.count; ++i)
			[res appendFormat:@" %@%@", [[mPlayers objectAtIndex:i] toString:withTypes :withCustomProperties], i<mPlayers.count-1?@",":@""];
	}
	return res;
}

@end