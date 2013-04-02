//
//  JGPhotonCloudUtil.m
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 12..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#import "JGPhotonCloudUtil.h"

@implementation JGPhotonCloudTypeUtil

+(NSValue *)intValue:(int)intValue_{
	return [NSValue valueWithBytes:&intValue_ objCType:@encode(int)];
}
+(NSValue *)longValue:(long)longValue_{
	return [NSValue valueWithBytes:&longValue_ objCType:@encode(long)];
}
+(NSValue *)floatValue:(float)floatValue_{
	return [NSValue valueWithBytes:&floatValue_ objCType:@encode(float)];
}
+(NSValue *)doubleValue:(double)doubleValue_{
	return [NSValue valueWithBytes:&doubleValue_ objCType:@encode(double)];
}

@end

@implementation NSDictionary(JGPhotonCloudExtension)

-(int)intValueForKey:(id)key_{
	int result_;
	[[self objectForKey:key_] getValue:&result_];
	return result_;
}
-(float)floatValueForKey:(id)key_{
	float result_;
	[[self objectForKey:key_] getValue:&result_];
	return result_;
}
-(double)doubleValueForKey:(id)key_{
	double result_;
	[[self objectForKey:key_] getValue:&result_];
	return result_;
}
-(CGPoint)CGPointValueForKey:(id)key_{
	return CGPointFromString([self objectForKey:key_]);
}
-(CGSize)CGSizeValueForKey:(id)key_{
	return CGSizeFromString([self objectForKey:key_]);
}
-(CGRect)CGRectValueForKey:(id)key_{
	return CGRectFromString([self objectForKey:key_]);
}

@end

@implementation NSMutableDictionary(JGPhotonCloudExtension)

-(void)setIntValue:(int)value_ forKey:(id)key_{
	[self setObject:[JGPhotonCloudTypeUtil intValue:value_] forKey:key_];
}
-(void)setFloatValue:(float)value_ forKey:(id)key_{
	[self setObject:[JGPhotonCloudTypeUtil floatValue:value_] forKey:key_];
}
-(void)setDoubleValue:(double)value_ forKey:(id)key_{
	[self setObject:[JGPhotonCloudTypeUtil doubleValue:value_] forKey:key_];
}
-(void)setCGPointValue:(CGPoint)value_ forKey:(id)key_{
	[self setObject:NSStringFromCGPoint(value_) forKey:key_];
}
-(void)setCGSizeValue:(CGSize)value_ forKey:(id)key_{
	[self setObject:NSStringFromCGSize(value_) forKey:key_];
}
-(void)setCGRectValue:(CGRect)value_ forKey:(id)key_{
	[self setObject:NSStringFromCGRect(value_) forKey:key_];
}

@end