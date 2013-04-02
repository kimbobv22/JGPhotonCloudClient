/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_OPERATION_RESPONSE_INTERNAL_H
#define __EG_OPERATION_RESPONSE_INTERNAL_H

#import "EGOperationResponse.h"
#include "OperationResponse.h"

@interface EGOperationResponse (Internal)

- (EGOperationResponse*) initWithCppOperationResponse:(const ExitGames::Photon::OperationResponse&)operationResponse;
+ (EGOperationResponse*) operationResponseWithCppOperationResponse:(const ExitGames::Photon::OperationResponse&)operationResponse;

@end

#endif