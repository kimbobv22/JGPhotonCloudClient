/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __OPERATION_REQUEST_INTERNAL_H
#define __OPERATION_REQUEST_INTERNAL_H

#import "OperationRequest.h"

@interface OperationRequest (Internal)

- (COperationRequest*) toC:(COperationRequest*)pCOperationRequest;

@end

#endif