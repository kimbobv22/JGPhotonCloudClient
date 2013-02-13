/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __UTILS_H
#define __UTILS_H

#import <Foundation/NSDictionary.h>
#import <Foundation/NSArray.h>
#import "Base.h"
#include "data_structures.h"

@interface Utils : Base

+ (EG_HashTable*) newEGHashTableFromNSDictionary:(NSDictionary*)origHash;
+ (EG_HashTable*) newEGHashTableFromNSDictionary:(NSDictionary*)origHash :(bool)protocol15;
+ (EG_Vector*) newEGVectorFromNSMutableArray:(NSMutableArray*)origVec;
+ (NSDictionary*) NSDictionaryFromEGHashTable:(EG_HashTable*)origHash;
+ (NSMutableArray*) NSMutableArrayFromEGVector:(EG_Vector*)origVec;

@end

#endif