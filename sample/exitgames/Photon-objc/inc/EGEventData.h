/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_EVENT_DATA_H
#define __EG_EVENT_DATA_H

#import "EGArray.h"

@interface EGEventData : NSObject
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