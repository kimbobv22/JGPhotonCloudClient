/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_PHOTON_PEER_INTERNAL_H
#define __EG_PHOTON_PEER_INTERNAL_H

#import "EGPhotonPeer.h"

@interface EGPhotonPeer (Internal)

@property (readonly) id<EGPhotonListener> InternalListener;

- (id) initWithPhotonListener:(id<EGPhotonListener>)listener :(bool)useTcp :(bool)useLitePeer;

@end

#endif