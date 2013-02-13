/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __CUSTOM_TYPE_H
#define __CUSTOM_TYPE_H

#include "data_structures.h"
#import "ToString.h"

@protocol CustomType <NSObject, ToString>

@property (readonly) nByte TypeCode;

- (id) initWithCustomType:(const id<CustomType>)toCopy;

- (void) cleanup;
- (bool) compare:(const id<CustomType>)other;
- (void) duplicate:(id<CustomType> const)retVal;
- (void) deserialize:(const nByte* const)pData :(short)length;
- (short) serialize:(nByte* const)retVal;

@end

#endif