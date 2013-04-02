/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_CUSTOM_TYPE_UTILS_H
#define __EG_CUSTOM_TYPE_UTILS_H

#import "EGBase.h"
#import "EGCustomType.h"
#include "defines.h"

@interface EGCustomTypeUtils : EGBase
{
}

+ (void) initClass:(nByte)typeCode :(Class<EGCustomType>)delegate;
+ (Class<EGCustomType>) getClassForCode:(nByte)typeCode;

@end

#endif