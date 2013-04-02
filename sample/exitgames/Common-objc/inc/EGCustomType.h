/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_CUSTOM_TYPE_H
#define __EG_CUSTOM_TYPE_H

#import "EGToString.h"
#include "defines.h"

@protocol EGCustomType <NSObject, EGToString>

@property (readonly) nByte TypeCode;

- (id) initWithCustomType:(const id<EGCustomType>)toCopy;

- (void) cleanup;
- (bool) compare:(const id<EGCustomType>)other;
- (void) duplicate:(id<EGCustomType> const)retVal;
- (void) deserialize:(const nByte* const)pData :(short)length;
- (short) serialize:(nByte* const)retVal;

@end

#endif