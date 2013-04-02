//
//  SimplePopup.mm
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 11..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#import "SimplePopup.h"

@implementation SimplePopup

-(id)init{
	CGSize winSize_ = [[CCDirector sharedDirector] winSize];
	if(!(self = [super initWithColor:ccc4(0, 0, 0, 180) width:winSize_.width*0.8f height:winSize_.height*0.8f])) return self;
	
	_backSprite = [CCSprite spriteWithTexture:[[CMMDrawingManager sharedManager] textureBatchBarWithFrameSeq:0 batchBarSeq:1 size:_contentSize]];
	[_backSprite setPosition:cmmFuncCommon_positionInParent(self, _backSprite)];
	[self addChild:_backSprite z:0];
	
	return self;
}

-(void)onEnter{
	[super onEnter];
	[self setPosition:cmmFuncCommon_positionInParent(_parent, self)];
}

@end

@implementation SimplePopupAlert
@synthesize content,button1;

+(id)popupWithContent:(NSString *)content_{
	return [[[self alloc] initWithContent:content_] autorelease];
}
-(id)initWithContent:(NSString *)content_{
	if(!(self = [super init])) return self;
	
	labelContent = [CMMFontUtil labelWithString:@" "];
	[labelContent setHorizontalAlignment:kCCTextAlignmentCenter];
	[labelContent setDimensions:CGSizeMake(_contentSize.width*0.9f, _contentSize.height-40.0f)];
	[labelContent setPosition:ccp(_contentSize.width*0.5f,_contentSize.height-[labelContent contentSize].height*0.5f)];
	[self addChild:labelContent z:1];
	
	CGSize buttonSize_ = CGSizeMake(120, 35);
	button1 = [CMMMenuItemL menuItemWithFrameSeq:0 batchBarSeq:0 frameSize:buttonSize_];
	[button1 setTitle:@"Confirm"];
	[button1 setPosition:ccp(_contentSize.width*0.5f,buttonSize_.height*0.5f+5.0f)];
	[self addChild:button1 z:1];
	
	[self setContent:content_];
	
	return self;
}

-(void)setContent:(NSString *)content_{
	[labelContent setString:content_];
}
-(NSString *)content{
	return [labelContent string];
}

@end

@implementation SimplePopupConfirm
@synthesize button2;

-(id)initWithContent:(NSString *)content_{
	if(!(self = [super initWithContent:content_])) return self;
	
	CGSize buttonSize_ = [button1 contentSize];
	
	[button1 setTitle:@"Yes"];
	[button1 setPosition:ccp(_contentSize.width*0.5f+buttonSize_.width*0.5f+5.0f,buttonSize_.height*0.5f+5.0f)];
	
	button2 = [CMMMenuItemL menuItemWithFrameSeq:0 batchBarSeq:0 frameSize:buttonSize_];
	[button2 setTitle:@"No"];
	[button2 setPosition:cmmFuncCommon_positionFromOtherNode(button1, button2, ccp(-1.0f,0.0f),ccp(-5.0f,0.0f))];
	[self addChild:button2 z:1];

	return self;
}

@end

@implementation SimpleLoadingLayer
@synthesize title;

-(id)initWithColor:(ccColor4B)color width:(GLfloat)w height:(GLfloat)h{
	if(!(self = [super initWithColor:color width:w height:h])) return self;
	[self setLazyCleanup:YES];
	
	labelTitle = [CMMFontUtil labelWithString:@""];
	[labelTitle setPosition:cmmFuncCommon_positionInParent(self, labelTitle)];
	[self addChild:labelTitle z:1];
	
	return self;
}

-(void)setTitle:(NSString *)title_{
	[labelTitle setString:title_];
}
-(NSString *)title{
	return [labelTitle string];
}

@end

static SimpleLoadingLayer *_staticSimpleLoadingLayer_ = nil;

@implementation CMMScene(SimplePopupExtension)

-(void)openPopupAlertWithContent:(NSString *)content_ closeBlock:(void(^)(SimplePopupAlert *alert_))closeBlock_{
	SimplePopupAlert *alert_ = [SimplePopupAlert popupWithContent:content_];
	[[alert_ button1] setCallback_pushup:^(id item_) {
		[alert_ close];
	}];
	[alert_ setCallback_didClose:(void(^)(CMMPopupLayer *popup_))closeBlock_];
	[self openPopupAtFirst:alert_];
}
-(void)openPopupAlertWithContent:(NSString *)content_{
	return [self openPopupAlertWithContent:content_ closeBlock:nil];
}

-(void)openPopupConfirmWithContent:(NSString *)content_ closeBlock:(void(^)(SimplePopupConfirm *confirm_, BOOL result_))closeBlock_{
	SimplePopupConfirm *confirm_ = [SimplePopupConfirm popupWithContent:content_];
	[[confirm_ button1] setCallback_pushup:^(id item_) {
		[confirm_ close];
		closeBlock_(confirm_,YES);
	}];
	[[confirm_ button2] setCallback_pushup:^(id item_) {
		[confirm_ close];
		closeBlock_(confirm_,NO);
	}];
	[self openPopupAtFirst:confirm_];
}

-(void)openLoadingLayerWithTitle:(NSString *)title_{
	if(!_staticSimpleLoadingLayer_){
		_staticSimpleLoadingLayer_ = [[SimpleLoadingLayer alloc] init];
	}
	
	[self closeLoadingLayer];
	[_staticSimpleLoadingLayer_ setTitle:title_];
	[self openPopupAtFirst:_staticSimpleLoadingLayer_];
}
-(void)closeLoadingLayer{
	[_staticSimpleLoadingLayer_ close];
}

@end