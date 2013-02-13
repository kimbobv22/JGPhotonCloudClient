/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __NSOBJECT_DEEP_COPY_EXTENSION_H
#define __NSOBJECT_DEEP_COPY_EXTENSION_H

#import <Foundation/NSObject.h>

@interface NSObject (CopyDeep)

- (id) copyDeep;

@end

#endif