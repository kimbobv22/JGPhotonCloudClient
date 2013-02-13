/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EVENT_DATA_INTERNAL_H
#define __EVENT_DATA_INTERNAL_H

#import "EventData.h"

@interface EventData (Internal)

- (EventData*) initWithCEventData:(CEventData*)cEventData;
+ (EventData*) eventDataWithCEventData:(CEventData*)cEventData;

@end

#endif