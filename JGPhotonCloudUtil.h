//
//  JGPhotonCloudUtil.h
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 12..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#import <Foundation/Foundation.h>

#pragma mark - Extension

@interface JGPhotonCloudTypeUtil : NSObject

+(NSValue *)intValue:(int)intValue_;
+(NSValue *)longValue:(long)longValue_;
+(NSValue *)floatValue:(float)floatValue_;
+(NSValue *)doubleValue:(double)doubleValue_;

@end

@interface NSDictionary(JGPhotonCloudExtension)

-(int)intValueForKey:(id)key_;
-(float)floatValueForKey:(id)key_;
-(double)doubleValueForKey:(id)key_;
-(CGPoint)CGPointValueForKey:(id)key_;
-(CGSize)CGSizeValueForKey:(id)key_;
-(CGRect)CGRectValueForKey:(id)key_;

@end

@interface NSMutableDictionary(JGPhotonCloudExtension)

-(void)setIntValue:(int)value_ forKey:(id)key_;
-(void)setFloatValue:(float)value_ forKey:(id)key_;
-(void)setDoubleValue:(double)value_ forKey:(id)key_;
-(void)setCGPointValue:(CGPoint)value_ forKey:(id)key_;
-(void)setCGSizeValue:(CGSize)value_ forKey:(id)key_;
-(void)setCGRectValue:(CGRect)value_ forKey:(id)key_;

@end

#pragma mark - Macro

#define jgFuncRespondsToSelector(_target_,_selector_) ((_target_) && [(_target_) respondsToSelector:(_selector_)])
