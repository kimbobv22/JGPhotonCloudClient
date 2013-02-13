/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __PHOTON_PEER_INTERNAL_H
#define __PHOTON_PEER_INTERNAL_H

#import "PhotonPeer.h"

@interface PhotonPeer (Internal)

@property (readonly) id<PhotonListener> InternalListener;

@end

#endif