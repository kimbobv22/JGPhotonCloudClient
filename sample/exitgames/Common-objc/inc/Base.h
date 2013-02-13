/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __BASE_H
#define __BASE_H

#import "BaseListener.h"
#import "ToString.h"

@interface Base : NSObject
{
}

+ (void) setListener:(id<BaseListener>)baseListener;
+ (void) debugReturn:(NSString*)string;

@end
#endif