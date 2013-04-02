/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */
 
/* ATTENTION:
EGArray and EGMutableArray are "typed" subclasses of NSArray:
They guarantee, that all their elements are of the same (semantic) type.
Using EGArray instead of NSArray will safe you traffic, as the type of the elements has to be serialized and sent just once for the whole array, not once per element like for a NSArray with potentially different element-types.
Listed below are special cases, for which the semantic type is not the same as the syntactic type. These are handled specially.

type-comparison-behavior:
- subclasses of NSValue, including NSValues theirselves, will only be handled as of equal type, if their payload types are equal
	-> meaning a NSValue, which is wrapping an int is not equal to one, which is wrapping a float
 	- Creation-methods of NSNumber are explicitly unsupported and will most definitely lead into unexpected types as these methods create objects with internal type representations not matching the
 		creation-method's name or the passed original type,
	- therefor please always use creation methods, which allow you to pass the encoding explicitly like
 		- (id)initWithBytes:(const void*)value objCType:(const char*)type;
 		+ (NSValue *)valueWithBytes:(const void*)value objCType:(const char*)type;
 		+ (NSValue *)value:(const void*)value withObjCType:(const char*)type;
- all subclasses of the Foundation class clusters will be handled as of equal type to instances of all other subclasses of the same class cluster
	- this affects NSString, NSDictionary, NSArray, NSData, NSSet and their subclasses
	- The reason therefor is, that you could not store instances of these class clusters in the same EGArray-instance otherwise,
 	  if their internal representation differs (which often is the case)
	-> this means for example, that uninitialized and initialized instances of NSString and NSMutableString as well as String-constants like @"" will all be treated as of the type NSString
- There is an exception to this behavior for NSArray: EGArray itself and all it's subclasses will not be treated as NSArray, but as EGArray,
 	- this is to guarantee, that an array of typed arrays will only hold typed arrays as elements
	-> a NSArray will not be treated as of the same type as an EGArray, but an EGArray will be treated as of the same type as an NSArray
- class clusters, which are not from the Foundation Framework, are not supported. Behavior for them is like comparing their syntactic types, not the semantic ones
	-> depending on implementation details of the specific cluster this can lead into unexpected behavior
- custom subclasses of Foundation class clusters will be handled as equal to all other subclasses of the class cluster, same as if they were internal subclases
- you can achieve, that an EGArray only accepts a specific subclass (and the subclasses of this subclass) of a class cluster, by initializing it with a type instead of with one or multiple elements
*/

#ifndef __EGARRAY_H
#define __EGARRAY_H

#include "defines.h"
#import <Foundation/NSArray.h>
#import "EGBase.h"

#ifdef _EG_IPHONE_PLATFORM
	#define IOS_OSX(ios, osx) ios
#else
	#define IOS_OSX(ios, osx) osx
#endif

// don't ever use NSAssert(), as it is evil! Details: NSAssert() isn't actually triggering an assert, but an InternalInconsistencyException, which could be caught by the app and that can lead into hard to find memory leaks (example: classic objC [[[Foo alloc] init] autorelease] and an NSAssert in init that gets catched at a higher level location in the calling stack -> autorelease won't ever be called and Foo leaks, although the intention of the NSAssert()-call has been to either terminate the app or continue with normal code execution, but under no circumstances to recover without calling the full code)
#define EG_ASSERT(condition, msg) DEBUG_RELEASE((!(condition)?DBGPRINTF_FATAL((msg).UTF8String):(void)0, assert(condition)), (void)0)

@interface EGArray : NSArray
{
@protected
	NSArray* mData;
	NSString* mType;
}

@property (readonly) NSArray* Data;
@property (readonly) NSString* Type;
@property (readonly) const char* const CType;
@property (readonly) const void* const CArray;

- (id) initWithType:(NSString*)type;
- (id) initWithCType:(const char* const)type;
+ (id) arrayWithType:(NSString*)type;
+ (id) arrayWithCType:(const char* const)type;
- (EGArray*) arrayByAddingObject:(id)anObject;
- (EGArray*) arrayByAddingObjectsFromArray:(NSArray*)otherArray;
+ (NSString*) getTypeFromObj:(id)obj;
+ (const char* const) getCTypeFromObj:(id)obj;

@end



@interface EGMutableArray : EGArray

@property (readwrite, copy) NSMutableArray* MutableData;

- (void)addObject:(id)anObject;
- (void)insertObject:(id)anObject atIndex:(NSUInteger)index;
- (void)removeLastObject;
- (void)removeObjectAtIndex:(NSUInteger)index;
- (void)replaceObjectAtIndex:(NSUInteger)index withObject:(id)anObject;

- (void)addObjectsFromArray:(NSArray*)otherArray;
- (void)exchangeObjectAtIndex:(NSUInteger)idx1 withObjectAtIndex:(NSUInteger)idx2;
- (void)removeAllObjects;
- (void)removeObject:(id)anObject inRange:(NSRange)range;
- (void)removeObject:(id)anObject;
- (void)removeObjectIdenticalTo:(id)anObject inRange:(NSRange)range;
- (void)removeObjectIdenticalTo:(id)anObject;
- (void)removeObjectsFromIndices:(NSUInteger*)indices numIndices:(NSUInteger)cnt IOS_OSX(NS_DEPRECATED(10_0, 10_6, 2_0, 4_0), DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER);
- (void)removeObjectsInArray:(NSArray*)otherArray;
- (void)removeObjectsInRange:(NSRange)range;
- (void)replaceObjectsInRange:(NSRange)range withObjectsFromArray:(NSArray*)otherArray range:(NSRange)otherRange;
- (void)replaceObjectsInRange:(NSRange)range withObjectsFromArray:(NSArray*)otherArray;
- (void)setArray:(NSArray*)otherArray;
- (void)sortUsingFunction:(NSInteger (*)(id, id, void*))compare context:(void*)context;
- (void)sortUsingSelector:(SEL)comparator;

- (void)insertObjects:(NSArray*)objects atIndexes:(NSIndexSet*)indexes IOS_OSX(NS_AVAILABLE(10_4, 2_0), AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER);
- (void)removeObjectsAtIndexes:(NSIndexSet*)indexes IOS_OSX(NS_AVAILABLE(10_4, 2_0), AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER);
- (void)replaceObjectsAtIndexes:(NSIndexSet*)indexes withObjects:(NSArray*)objects IOS_OSX(NS_AVAILABLE(10_4, 2_0), AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER);

#if NS_BLOCKS_AVAILABLE
- (void)sortUsingComparator:(NSComparator)cmptr IOS_OSX(NS_AVAILABLE(10_6, 4_0), AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER);
- (void)sortWithOptions:(NSSortOptions)opts usingComparator:(NSComparator)cmptr IOS_OSX(NS_AVAILABLE(10_6, 4_0), AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER);
#endif

+ (id)arrayWithCapacity:(NSUInteger)numItems :(NSString*)type;
+ (id)arrayWithCapacityAndCType:(NSUInteger)numItems :(const char* const)type;
- (id)initWithCapacity:(NSUInteger)numItems :(NSString*)type;
- (id)initWithCapacityAndCType:(NSUInteger)numItems :(const char* const)type;

@end

#endif