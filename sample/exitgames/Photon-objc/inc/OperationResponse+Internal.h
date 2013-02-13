/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __OPERATION_RESPONSE_INTERNAL_H
#define __OPERATION_RESPONSE_INTERNAL_H

#import "OperationResponse.h"

@interface OperationResponse (Internal)

- (OperationResponse*) initWithCOperationResponse:(COperationResponse*)cOperationResponse;
+ (OperationResponse*) operationResponseWithCOperationResponse:(COperationResponse*)cOperationResponse;

@end

#endif