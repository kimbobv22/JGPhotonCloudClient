/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __NSSTRING_UTF32_EXTENSION_H
#define __NSSTRING_UTF32_EXTENSION_H

#import <Foundation/NSString.h>

#include "defines.h"

@interface NSString (UTF32)

+ (NSString*) stringWithUTF32String:(const EG_CHAR* const)str;
- (NSString*) initWithUTF32String:(const EG_CHAR* const)str;
- (const EG_CHAR* const) UTF32String;
- (EG_CHAR*) mallocedUTF32String; // the caller is responsible for freeing the returnvalue of this method!

@end

#endif