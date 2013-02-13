/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __PHOTON_BASE_LISTENER_H
#define __PHOTON_BASE_LISTENER_H

#import <Foundation/Foundation.h>
#import "OperationResponse.h"
#import "EventData.h"

@protocol PhotonBaseListener

- (void) debugReturn:(PhotonPeer_DebugLevel)debugLevel :(NSString*)string;

@end

/** @file */

#endif