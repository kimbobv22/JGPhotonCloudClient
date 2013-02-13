/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __CUSTOM_TYPE_UTILS_H
#define __CUSTOM_TYPE_UTILS_H

#import "Base.h"
#include "data_structures.h"
#import "CustomType.h"

@interface CustomTypeUtils : Base
{
}

+ (void) initClass:(nByte)typeCode :(Class<CustomType>)delegate;
+ (Class<CustomType>) getClassForCode:(nByte)typeCode;

@end

#endif