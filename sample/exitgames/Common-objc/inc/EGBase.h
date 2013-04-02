/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_BASE_H
#define __EG_BASE_H

#import "EGBaseListener.h"
#import "EGToString.h"

@interface EGBase : NSObject
{
}

+ (void) setListener:(id<EGBaseListener>)baseListener;
+ (void) debugReturn:(int)debugLevel :(NSString*)string;

@end
#endif