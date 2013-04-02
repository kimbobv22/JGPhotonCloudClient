/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_OPERATION_REQUEST_H
#define __EG_OPERATION_REQUEST_H

#include "EGOperationRequest.h"
#import "EGArray.h"

@interface EGOperationRequest : NSObject
{
@private
	nByte mOperationCode;
	NSDictionary* mParameters;
}

@property (readonly) nByte OperationCode;
@property (readonly) const NSDictionary* Parameters;

- (EGOperationRequest*) initWithOperationCode:(nByte)operationCode;
- (EGOperationRequest*) initWithOperationCode:(nByte)operationCode :(NSDictionary*)parameters;
+ (EGOperationRequest*) operationRequestWithOperationCode:(nByte)operationCode;
+ (EGOperationRequest*) operationRequestWithOperationCode:(nByte)operationCode :(NSDictionary*)parameters;
- (NSArray*) allKeys;
- (NSArray*) allValues;
- (NSString*) toString; // = false, = false
- (NSString*) toString:(bool)withParameters; // = false
- (NSString*) toString:(bool)withParameters :(bool)withParameterTypes;
- (id) objectForKey:(id)key;

@end

#endif