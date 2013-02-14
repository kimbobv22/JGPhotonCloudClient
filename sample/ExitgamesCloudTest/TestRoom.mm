//
//  TestRoom.mm
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 11..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#import "TestRoom.h"

@implementation TestPlayer
@synthesize playerNr;

@end

@implementation TestRoom

-(id)initWithColor:(ccColor4B)color width:(GLfloat)w height:(GLfloat)h{
	if(!(self = [super initWithColor:color width:w height:h])) return self;
	
	playerSpriteList = [[CCArray alloc] init];
	
	btnExit = [CMMMenuItemL menuItemWithFrameSeq:0 batchBarSeq:0];
	[btnExit setTitle:@"Exit"];
	[btnExit setCallback_pushup:^(id item_) {
		[self _leaveRoom];
	}];
	CGSize btnSize_ = [btnExit contentSize];
	[btnExit setPosition:ccp(btnSize_.width*0.5f+5.0f,btnSize_.height*0.5f+5.0f)];
	[self addChild:btnExit];
	
	//add others.
	currentPhotonRoom = [[JGPhotonCloudClient defaultClient] currentlyRoom];
	LoadBalancingMutablePlayer *myPeer_ = [[JGPhotonCloudClient defaultClient] currentlyPlayer];
	
	EGArray *players_ = [currentPhotonRoom Players];
	for(LoadBalancingPlayer *player_ in players_){
		int playerNr_ = [player_ Number];
		if(playerNr_ != [myPeer_ Number]){
			[self addPlayerSprite:[player_ Number]];
		}
	}
	
	//add me.
	me = [TestPlayer spriteWithFile:@"Player.png"];
	[me setPlayerNr:[myPeer_ Number]];
	[me setColor:ccc3(255, 0, 0)];
	[me setPosition:ccp(_contentSize.width/2.0f,_contentSize.height/2.0f)];
	[self addChild:me z:1];
	[self _sendEvent_position:[me position]];
	
	_sendPointDelayTime = 1.0f;
	_currentSendPointDelayTime = 0.0f;
	
	//add simple chat
	labelChat = [CMMFontUtil labelWithString:@" "];
	[labelChat setDimensions:CGSizeMake(_contentSize.width, [labelChat contentSize].height)];
	[labelChat setHorizontalAlignment:kCCTextAlignmentLeft];
	
	textfieldChat = [CMMControlItemText controlItemTextWithFrameSeq:0 width:_contentSize.width*0.4f];
	[textfieldChat setPlaceHolder:@"enter message"];
	[textfieldChat setCallback_whenItemValueChanged:^(NSString *itemValue_) {
		[btnSendChat setEnable:([itemValue_ length] != 0)];
	}];
	
	btnSendChat = [CMMMenuItemL menuItemWithFrameSeq:0 batchBarSeq:0 frameSize:CGSizeMake(100, 35)];
	[btnSendChat setTitle:@"Send"];
	[btnSendChat setEnable:NO];
	[btnSendChat setCallback_pushup:^(id item_) {
		[self _sendEvent_chatMsg:[textfieldChat itemValue]];
		[textfieldChat setItemValue:nil];
	}];
	
	[labelChat setPosition:ccp(_contentSize.width*0.5f,_contentSize.height-[labelChat contentSize].height*0.5f)];
	[btnSendChat setPosition:ccp(_contentSize.width-[btnSendChat contentSize].width*0.5f-5.0f,[btnSendChat contentSize].height*0.5f+5.0f)];
	[textfieldChat setPosition:cmmFuncCommon_positionFromOtherNode(btnSendChat, textfieldChat, ccp(-1.0f,0.0),ccp(-10.0f,0.0f))];
	
	[self addChild:labelChat z:1];
	[self addChild:btnSendChat z:1];
	[self addChild:textfieldChat z:1];

	[[JGPhotonCloudClient defaultClient] setRoomDelegate:self];
	[self scheduleUpdate];
	
	return self;
}

-(void)_setUIEnable:(BOOL)bool_{
	[btnExit setEnable:bool_];
}

-(void)_leaveRoom{
	[[CMMScene sharedScene] openLoadingLayerWithTitle:@"Leaving from room..."];
	[self _setUIEnable:NO];
	[[JGPhotonCloudClient defaultClient] leaveRoom];
}

-(void)addPlayerSprite:(int)playerNr_{
	if([self playerSpriteAtPlayerNr:playerNr_]) return;
	
	TestPlayer *player_ = [TestPlayer spriteWithFile:@"Player.png"];
	[player_ setPlayerNr:playerNr_];
	[player_ setPosition:ccp(_contentSize.width/2.0f,_contentSize.height/2.0f)];
	[playerSpriteList addObject:player_];
	[self addChild:player_ z:1];
}
-(void)removePlayerSprite:(int)playerNr_{
	TestPlayer *player_ = [self playerSpriteAtPlayerNr:playerNr_];
	if(!player_) return;
	[playerSpriteList removeObject:player_];
	[self removeChild:player_ cleanup:YES];
}
-(TestPlayer *)playerSpriteAtPlayerNr:(int)playerNr_{
	ccArray *data_ = playerSpriteList->data;
	uint count_ = data_->num;
	for(uint index_=0;index_<count_;++index_){
		TestPlayer *player_ = data_->arr[index_];
		if([player_ playerNr] == playerNr_){
			return player_;
		}
	}
	
	return nil;
}
-(void)updatePlayerPoint:(int)playerNr_ point:(CGPoint)point_{
	TestPlayer *player_ = [self playerSpriteAtPlayerNr:playerNr_];
	[player_ setPosition:point_];
}

-(void)update:(ccTime)dt_{
	_currentSendPointDelayTime += dt_;
	
	if(_isOnTouch){
		CGPoint myPoint_ = [me position];
		CGPoint targetPoint_ = _touchPoint;
		
		myPoint_ = ccpAdd(myPoint_, ccpMult(ccpSub(targetPoint_, myPoint_), dt_*2.0f));
		[me setPosition:myPoint_];
		
		[self _sendEvent_position:myPoint_];
		_currentSendPointDelayTime = 0.0f;
	}
	
	if(_currentSendPointDelayTime > _sendPointDelayTime){
		_currentSendPointDelayTime = 0.0f;
		[self _sendEvent_position:[me position]];
	}
}

-(void)_sendEvent_position:(CGPoint)point_{
	NSMutableDictionary *pointDic_ = [NSMutableDictionary dictionary];
	[pointDic_ setFloatValue:point_.x forKey:@"x"];
	[pointDic_ setFloatValue:point_.y forKey:@"y"];
	[[JGPhotonCloudClient defaultClient] sendEventWithEventCode:varTestRoom_eventCode_position parameters:pointDic_ isReliable:NO];
}
-(void)_sendEvent_chatMsg:(NSString *)msg_{
	if([msg_ length] == 0) return;
	[[JGPhotonCloudClient defaultClient] sendEventWithEventCode:varTestRoom_eventCode_chat parameters:[NSDictionary dictionaryWithObjectsAndKeys:msg_,@"msg", nil]];
}

#pragma mark - touch dispatcher

-(void)touchDispatcher:(CMMTouchDispatcher *)touchDispatcher_ whenTouchBegan:(UITouch *)touch_ event:(UIEvent *)event_{
	[super touchDispatcher:touchDispatcher_ whenTouchBegan:touch_ event:event_];
	
	if([touchDispatcher touchCount] == 0){
		_isOnTouch = YES;
		_touchPoint = [CMMTouchUtil pointFromTouch:touch_ targetNode:self];
	}
}
-(void)touchDispatcher:(CMMTouchDispatcher *)touchDispatcher_ whenTouchMoved:(UITouch *)touch_ event:(UIEvent *)event_{
	[super touchDispatcher:touchDispatcher_ whenTouchMoved:touch_ event:event_];
	if(_isOnTouch){
		_touchPoint = [CMMTouchUtil pointFromTouch:touch_ targetNode:self];
	}
}
-(void)touchDispatcher:(CMMTouchDispatcher *)touchDispatcher_ whenTouchEnded:(UITouch *)touch_ event:(UIEvent *)event_{
	[super touchDispatcher:touchDispatcher_ whenTouchEnded:touch_ event:event_];
	_isOnTouch = NO;
}
-(void)touchDispatcher:(CMMTouchDispatcher *)touchDispatcher_ whenTouchCancelled:(UITouch *)touch_ event:(UIEvent *)event_{
	[super touchDispatcher:touchDispatcher_ whenTouchCancelled:touch_ event:event_];
	_isOnTouch = NO;
}

#pragma mark - photon delegate

-(void)jgPhotonCloudLeavedRoom:(JGPhotonCloudClient *)client_{
	CCLOG(@"PhotonCloudLeavedRoom");
	[[CMMScene sharedScene] openLoadingLayerWithTitle:@"Joining to lobby..."];
	[client_ joinLobby];
	//[[CMMScene sharedScene] pushStaticLayerItemAtKey:varTestLobby_staticKey];
}
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ failedToLeaveRoom:(int)errorCode_ errorString:(NSString *)errorString_{
	[[CMMScene sharedScene] openPopupAlertWithContent:[NSString stringWithFormat:@"PhotonCloud failedToLeaveRoom [%d] %@",errorCode_,errorString_]];
}

-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ eventCode:(nByte)eventCode_ receivedEvent:(const NSDictionary *)event_{
	switch(eventCode_){
		case varTestRoom_eventCode_position:{
			TestPlayer *player_ = [self playerSpriteAtPlayerNr:playerNr_];
			if(player_){
				CGPoint targetPoint_ = ccp([event_ floatValueForKey:@"x"],[event_ floatValueForKey:@"y"]);
				[player_ setPosition:targetPoint_];
			}

			break;
		}
		case varTestRoom_eventCode_chat:{
			EGArray *playerList_ = [currentPhotonRoom Players];
			NSString *playerName_ = @"unknown";
			for(LoadBalancingPlayer *player_ in playerList_){
				if([player_ Number] == playerNr_){
					playerName_ = [player_ Name];
					break;
				}
			}
			
			[labelChat setString:[NSString stringWithFormat:@"[%@ says] : %@",playerName_	,[event_ objectForKey:@"msg"]]];
			break;
		}
		default: break;
	}
}

-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ whenPeerJoinedRoom:(NSString *)roomName_ playerData:(const LoadBalancingPlayer *)playerData_{
	if(playerNr_ == [me playerNr]) return;
	CCLOG(@"PhotonCloud whenPeerJoinedRoom %d",playerNr_);
	[self addPlayerSprite:playerNr_];
}
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ whenPeerLeavedRoom:(NSString *)roomName_{
	CCLOG(@"PhotonCloud whenPeerLeavedRoom %d",playerNr_);
	[self removePlayerSprite:playerNr_];
}

#pragma mark -

-(void)cleanup{
	[[JGPhotonCloudClient defaultClient] setRoomDelegate:nil];
	[super cleanup];
}

-(void)dealloc{
	[playerSpriteList release];
	[super dealloc];
}

@end
