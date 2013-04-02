/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_OPERATION_REQUEST_INTERNAL_H
#define __EG_OPERATION_REQUEST_INTERNAL_H

#import "EGOperationRequest.h"
#include "OperationRequest.h"

@interface EGOperationRequest (Internal)

- (ExitGames::Photon::OperationRequest*) toCPP;

@end

#endif