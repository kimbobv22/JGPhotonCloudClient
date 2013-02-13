/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2012 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#import "Foundation/NSException.h"
#import "LoadBalancingPeer+Internal.h"
#import "LoadBalancingPlayer+Internal.h"
#import "LoadBalancingroom+Internal.h"

/** @file LoadBalancingPlayer.h */

using namespace LoadBalancing;

@interface LoadBalancingPlayer (Private)

- (NSString*) payloadToString:(bool)withTypes :(bool)withCustomProperties;

@end



@implementation LoadBalancingPlayer

@synthesize Number = mNumber;

- (NSString*) Name
{
	return [[mName copy] autorelease];
}

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
	return [NSString stringWithFormat:@"%d={%@}", mNumber, [self payloadToString:withTypes :withCustomProperties]];
}

@end



@implementation LoadBalancingPlayer (Internal)

- (id) initLoadBalancingPlayer:(int)number :(NSDictionary*)properties :(const LoadBalancingMutableRoom* const)room
{
	self = [super init];
	mNumber = number;
	mRoom = room;
	mCustomProperties = [[NSMutableDictionary alloc] init];
	return [self cacheProperties:properties];
}

+ (id) loadBalancingPlayer:(int)number :(NSDictionary*)properties :(const LoadBalancingMutableRoom* const)room
{
	return [[[self alloc] initLoadBalancingPlayer:number :properties :room] autorelease];
}

- (id) cacheProperties:(NSDictionary*)properties
{
	if([properties objectForKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]])
	{
		[mName release];
		mName = [[properties objectForKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]] retain];
	}
	[mCustomProperties addEntriesFromDictionary:[LoadBalancingPeer stripToCustomProperties:properties]];
	mCustomProperties = [[LoadBalancingPeer stripKeysWithNullValues:[mCustomProperties autorelease]] retain];
	return self;
}

- (BOOL) isEqual:(id)object
{
	return [object isKindOfClass:[LoadBalancingPlayer class]] && self.Number == ((LoadBalancingPlayer*)object).Number;
}

- (NSUInteger)hash
{
	return self.Number;
}

- (void) dealloc
{
	[mCustomProperties release];
	[mName release];
	[super dealloc];
}

@end



@implementation LoadBalancingPlayer (Private)

- (NSString*) payloadToString:(bool)withTypes :(bool)withCustomProperties
{
	NSString* res = [NSString stringWithFormat:@"name: %@", mName];
	if(withCustomProperties && mCustomProperties.count)
		res = [res stringByAppendingFormat:@", props: %@", [mCustomProperties toString:withTypes]];
	return res;
}

@end



@implementation LoadBalancingMutablePlayer

- (NSString*) Name
{
	return [[mName copy] autorelease];
}

- (void) setName:(NSMutableString*)name
{
	if(mName == name)
		return;
	[mName release];
	[mLoadBalancingPeer opSetPropertiesOfPlayer:mNumber :[NSDictionary dictionaryWithObject:mName=[name copy] forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]]];
}

- (bool) IsMasterClient
{
	return mRoom?mNumber==mRoom.MasterClientID:false;
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
		[mLoadBalancingPeer opSetPropertiesOfPlayer:mNumber :stripDict];	
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

@end



@implementation LoadBalancingMutablePlayer (Internal)

- (id) initLoadBalancingPlayer:(int)number :(NSDictionary*)properties :(const LoadBalancingMutableRoom* const)room
{
	NSAssert(false, @"not implemented");
	return nil;
}

- (id) initLoadBalancingPlayer:(int)number :(NSDictionary*)properties :(const LoadBalancingMutableRoom* const)room :(LoadBalancingPeer*)peer
{
	self = [super initLoadBalancingPlayer:number :properties: room];
	mLoadBalancingPeer = [peer retain];
	return self;
}

+ (id) loadBalancingPlayer:(int)number :(NSDictionary*)properties :(const LoadBalancingMutableRoom* const)room :(LoadBalancingPeer*)peer
{
	return [[[self alloc] initLoadBalancingPlayer:number :properties :room :peer] autorelease];
}

- (void) cleanCustomProperties
{
	[mCustomProperties removeAllObjects];
}

- (void) dealloc
{
	[mLoadBalancingPeer release];
	[super dealloc];
}

@end