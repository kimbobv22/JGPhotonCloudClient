/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */
 
#ifndef __EGDICTIONARY_H
#define __EGDICTIONARY_H

#import <Foundation/NSDictionary.h>
#import <Foundation/NSArray.h>
#import "EGArray.h"

@interface EGDictionary : NSDictionary
{
@protected
	NSDictionary* mData;
	EGArray* mKeyTypes;
	EGArray* mValTypes;
	EGArray* mDimensions;
}

@property (readonly) NSDictionary* Data;
@property (readonly) NSString* KeyType;
@property (readonly) NSString* ValType;
@property (readonly) EGArray* KeyTypes;
@property (readonly) EGArray* ValTypes;
@property (readonly) EGArray* Dimensions;
@property (readonly) const char* const CKeyType;
@property (readonly) const char* const CValType;

- (id) initWithKeyType:(NSString*)keyType :(NSString*)valType;
- (id) initWithCKeyType:(const char* const)keyType :(const char* const)valType;
- (id) initWithKeyTypes:(NSString*[])keys valueTypes:(NSString*[])vals count:(unsigned int)cnt;
+ (id) dictionaryWithKeyType:(NSString*)keyType valueType:(NSString*)valType;
+ (id) dictionaryWithCKeyType:(const char* const)keyType valueType:(const char* const)valType;
+ (NSString*) getTypeFromObj:(id)obj;
+ (const char* const) getCTypeFromObj:(id)obj;
+ (id) dictionaryWithKeyTypes:(NSString*[])keys valueTypes:(NSString*[])vals count:(unsigned int)cnt;

+ (id) dictionaryWithKeyType:(NSString*)keyType valueType:(NSString*)valType valueDimensions:(unsigned int)dimension;
- (id) initWithKeyType:(NSString*)keyType valueType:(NSString*)valType valueDimensions:(unsigned int)dimension;
+ (id) dictionaryWithKeyTypes:(NSString*[])keys valueTypes:(NSString*[])vals valueDimensions:(unsigned int*)dimension count:(unsigned int)cnt;
- (id) initWithKeyTypes:(NSString*[])keys valueTypes:(NSString*[])vals valueDimensions:(unsigned int*)dimension count:(unsigned int)cnt;

@end


@interface EGMutableDictionary : EGDictionary

@property (readwrite, copy) NSMutableDictionary* MutableData;

- (void)setObject:(id)anObject forKey:(id)aKey;
- (void)setValue:(id)value forKey:(NSString*)key;
- (void)addEntriesFromDictionary:(NSDictionary *)otherDictionary;
- (void)setDictionary:(NSDictionary*)otherDictionary;
- (void)removeObjectForKey:(id)aKey;
- (void)removeAllObjects;
- (void)removeObjectsForKeys:(NSArray*)keyArray;

+ (id)dictionaryWithCapacity:(NSUInteger)numItems;
- (id)initWithCapacity:(NSUInteger)numItems;

@end

#endif