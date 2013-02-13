/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_TIME_H
#define __EG_TIME_H

#include "data_structures.h"
#import "Base.h"

@interface EGTime : Base
{
}

+ (bool) less:(int)firstTime:(int)secondTime;
+ (bool) greater:(int)firstTime:(int)secondTime;
+ (bool) lessOrEqual:(int)firstTime:(int)secondTime;
+ (bool) greaterOrEqual:(int)firstTime:(int)secondTime;
+ (int) difference:(int)firstTime:(int)secondTime;

@end

#endif