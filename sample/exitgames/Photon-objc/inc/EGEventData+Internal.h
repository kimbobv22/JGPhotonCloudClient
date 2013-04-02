/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_EVENT_DATA_INTERNAL_H
#define __EG_EVENT_DATA_INTERNAL_H

#import "EGEventData.h"
#include "EventData.h"

@interface EGEventData (Internal)

- (EGEventData*) initWithCppEventData:(const ExitGames::Photon::EventData&)eventData;
+ (EGEventData*) eventDataWithCppEventData:(const ExitGames::Photon::EventData&)eventData;

@end

#endif