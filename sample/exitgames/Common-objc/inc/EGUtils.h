/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_UTILS_H
#define __EG_UTILS_H

#import <Foundation/NSDictionary.h>
#import <Foundation/NSArray.h>
#import "EGBase.h"

#include "Hashtable.h"

@interface EGUtils: EGBase

+ (ExitGames::Common::Hashtable*) newEGHashTableFromNSDictionary:(NSDictionary*)origHash;
+ (ExitGames::Common::Hashtable*) newEGHashTableFromNSDictionary:(NSDictionary*)origHash :(bool)protocol15;
+ (ExitGames::Common::JVector<ExitGames::Common::Object>*) newEGVectorFromNSMutableArray:(NSMutableArray*)origVec;
+ (NSDictionary*) NSDictionaryFromEGHashTable:(const ExitGames::Common::Hashtable*)origHash;
+ (NSMutableArray*) NSMutableArrayFromEGVector:(ExitGames::Common::JVector<ExitGames::Common::Object>*)origVec;

@end

#endif