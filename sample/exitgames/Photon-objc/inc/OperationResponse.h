/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __OPERATION_RESPONSE_H
#define __OPERATION_RESPONSE_H

#include "COperationResponse.h"
#import "EGArray.h"

@interface OperationResponse : NSObject
{
@private
	nByte mOperationCode;
	short mReturnCode;
	NSString* mDebugMessage;
	NSDictionary* mParameters;
}

@property (readonly) nByte OperationCode;
@property (readonly) short ReturnCode;
@property (readonly) const NSString* DebugMessage;
@property (readonly) const NSDictionary* Parameters;

- (NSArray*) allKeys;
- (NSArray*) allValues;
- (NSString*) toString; // = false, = false, = false
- (NSString*) toString:(bool)withDebugMessage; // = false, = false
- (NSString*) toString:(bool)withDebugMessage :(bool)withParameters; // = false
- (NSString*) toString:(bool)withDebugMessage :(bool)withParameters :(bool)withParameterTypes;
- (id) objectForKey:(id)key;

@end

#endif