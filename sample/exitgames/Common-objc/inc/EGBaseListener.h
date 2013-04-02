/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_BASE_LISTENER_H
#define __EG_BASE_LISTENER_H

#import <Foundation/NSString.h>
//#import "Enums/DebugLevel.h"

@protocol EGBaseListener

- (void) debugReturn:(int)debugLevel :(NSString*)string;

@end

/** @file */

#endif