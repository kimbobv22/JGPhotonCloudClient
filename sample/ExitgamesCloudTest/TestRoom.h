//
//  TestRoom.h
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 11..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#import "CMMHeader.h"
#import "SimplePopup.h"
#import "TestLobby.h"

@interface TestPlayer : CCSprite{
	int playerNr;
}

@property (nonatomic, readwrite) int playerNr;

@end

#define varTestRoom_eventCode_position 100
#define varTestRoom_eventCode_chat 110

@interface TestRoom : CMMLayer<JGPhotonCloudRoomDelegate>{
	LoadBalancingMutableRoom *currentPhotonRoom;
	TestPlayer *me;
	
	BOOL _isOnTouch;
	CGPoint _touchPoint;
	ccTime _sendPointDelayTime,_currentSendPointDelayTime;
	
	CCArray *playerSpriteList;
	
	CCLabelTTF *labelChat;
	CMMControlItemText *textfieldChat;
	CMMMenuItemL *btnSendChat;
	
	CMMMenuItemL *btnExit;
}

-(void)addPlayerSprite:(int)playerNr_;
-(void)removePlayerSprite:(int)playerNr_;
-(TestPlayer *)playerSpriteAtPlayerNr:(int)playerNr_;
-(void)updatePlayerPoint:(int)playerNr_ point:(CGPoint)point_;

@end
