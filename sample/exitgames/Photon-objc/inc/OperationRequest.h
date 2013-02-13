/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __OPERATION_REQUEST_H
#define __OPERATION_REQUEST_H

#include "COperationRequest.h"
#import "EGArray.h"

@interface OperationRequest : NSObject
{
@private
	nByte mOperationCode;
	NSDictionary* mParameters;
}

@property (readonly) nByte OperationCode;
@property (readonly) const NSDictionary* Parameters;

- (OperationRequest*) initWithOperationCode:(nByte)operationCode;
- (OperationRequest*) initWithOperationCode:(nByte)operationCode :(NSDictionary*)parameters;
+ (OperationRequest*) operationRequestWithOperationCode:(nByte)operationCode;
+ (OperationRequest*) operationRequestWithOperationCode:(nByte)operationCode :(NSDictionary*)parameters;
- (NSArray*) allKeys;
- (NSArray*) allValues;
- (NSString*) toString; // = false, = false
- (NSString*) toString:(bool)withParameters; // = false
- (NSString*) toString:(bool)withParameters :(bool)withParameterTypes;
- (id) objectForKey:(id)key;

@end

#endif