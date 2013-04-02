/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_PHOTON_LISTENER_H
#define __EG_PHOTON_LISTENER_H

#import "EGBaseListener.h"
#import "EGOperationResponse.h"
#import "EGEventData.h"

@protocol EGPhotonListener <EGBaseListener>

- (void) onOperationResponse:(EGOperationResponse*)operationResponse;
- (void) onStatusChanged:(int)statusCode;
- (void) onEvent:(EGEventData*)eventData;

@end

/** @file */

#endif