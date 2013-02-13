/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __PHOTON_LISTENER_H
#define __PHOTON_LISTENER_H

#import "PhotonBaseListener.h"

@protocol PhotonListener <PhotonBaseListener>

- (void) onOperationResponse:(OperationResponse*)operationResponse;
- (void) onStatusChanged:(int)statusCode;
- (void) onEvent:(EventData*)eventData;

@end

/** @file */

#endif