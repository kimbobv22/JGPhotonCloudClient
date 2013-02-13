/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EVENT_DATA_H
#define __EVENT_DATA_H

#include "CEventData.h"
#import "EGArray.h"

@interface EventData : NSObject
{
@private
	nByte mCode;
	NSDictionary* mParameters;
}

@property (readonly) nByte Code;
@property (readonly) const NSDictionary* Parameters;

- (NSArray*) allKeys;
- (NSArray*) allValues;
- (NSString*) toString; // = false, = false
- (NSString*) toString:(bool)withParameters; // = false
- (NSString*) toString:(bool)withParameters :(bool)withParameterTypes;
- (id) objectForKey:(id)key;

@end

#endif