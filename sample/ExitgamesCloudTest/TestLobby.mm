//
//  TestLobby.mm
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 11..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#import "TestLobby.h"
#import "TestRoom.h"
#import "TestStarter.h"

@implementation TestLobby

-(id)initWithColor:(ccColor4B)color width:(GLfloat)w height:(GLfloat)h{
	if(!(self = [super initWithColor:color width:w height:h])) return self;
	
	CGSize viewRoomListSize_ = CGSizeMake(_contentSize.width*0.8f, _contentSize.height*0.6f);
	viewRoomList = [CMMScrollMenuV scrollMenuWithFrameSeq:0 batchBarSeq:1 frameSize:viewRoomListSize_];
	[viewRoomList setPosition:cmmFuncCommon_positionInParent(self, viewRoomList, ccp(0.5,0.6f))];
	
	btnCreate = [CMMMenuItemL menuItemWithFrameSeq:0 batchBarSeq:0];
	[btnCreate setTitle:@"Create"];
	[btnCreate setCallback_pushup:^(id item_) {
		[self _createRoom];
	}];
	[btnCreate setPosition:cmmFuncCommon_positionFromOtherNode(viewRoomList, btnCreate, ccp(1.0f,-1.0f),ccp(-[btnCreate contentSize].width,-10.0f))];
	
	btnRefresh = [CMMMenuItemL menuItemWithFrameSeq:0 batchBarSeq:0];
	[btnRefresh setTitle:@"Refresh"];
	[btnRefresh setCallback_pushup:^(id item_) {
		[self _loadRoomList];
	}];
	
	[btnRefresh setPosition:cmmFuncCommon_positionFromOtherNode(btnCreate, btnRefresh, ccp(-1.0f,0.0f),ccp(-10.0f,0.0f))];
	
	[self addChild:viewRoomList];
	[self addChild:btnCreate];
	[self addChild:btnRefresh];
	
	JGPhotonCloudClient *pCloudClient_ = [JGPhotonCloudClient defaultClient];
	[pCloudClient_ setLobbyDelegate:self];
	
	return self;
}

-(void)onEnter{
	[super onEnter];
	[[CMMScene sharedScene] openLoadingLayerWithTitle:@"Joining to lobby..."];
}
-(void)sceneDidEndTransition:(CMMScene *)scene_{
	[[JGPhotonCloudClient defaultClient] joinLobby];
}

-(void)_setUIEnable:(BOOL)bool_{
	[viewRoomList setCanSelectItem:bool_];
	[btnCreate setEnable:bool_];
	[btnRefresh setEnable:bool_];
}

-(void)_loadRoomList{
	[self _setUIEnable:NO];
	
	[viewRoomList removeAllItems];
	JGPhotonCloudClient *client_ = [JGPhotonCloudClient defaultClient];
	
	CGSize menuItemSize_ = CGSizeMake([viewRoomList contentSize].width, 35);
	NSDictionary *roomDic_ = [client_ roomList];
	NSArray *roomNames_ = [roomDic_ allKeys];
	for(NSString *roomName_ in roomNames_){
		EGLoadBalancingRoom *room_ = [roomDic_ objectForKey:roomName_];
		NSString *roomTitle_ = [[room_ CustomProperties] objectForKey:varTestLobby_roomProperties_roomTitle];
		roomTitle_ = roomTitle_? roomTitle_ : @"none room title";
		roomTitle_ = [NSString stringWithFormat:@"%@ (%d/%d)",roomTitle_,[room_ PlayerCount],[room_ MaxPlayers]];
		
		CMMMenuItemL *roomItem_ = [CMMMenuItemL menuItemWithFrameSeq:0 batchBarSeq:0 frameSize:menuItemSize_];
		[roomItem_ setTitle:roomTitle_];
		[roomItem_ setCallback_pushup:^(id item_) {
			[self _joinRoom:roomName_];
		}];
		
		[viewRoomList addItem:roomItem_];
	}
	
	[self _setUIEnable:YES];
}
-(void)_createRoom{
	[[CMMScene sharedScene] openLoadingLayerWithTitle:@"Creating room..."];
	[self _setUIEnable:NO];
	EGLoadBalancingMutablePlayer *currentPlayer_ = [[JGPhotonCloudClient defaultClient] currentlyPlayer];
	[[JGPhotonCloudClient defaultClient] createRoomWithMaxPlayers:4 isVisible:YES isOpen:YES customProperties:[NSDictionary dictionaryWithObjectsAndKeys:[NSString stringWithFormat:@"%@'s room",[currentPlayer_ Name]],varTestLobby_roomProperties_roomTitle, nil]];
}
-(void)_joinRoom:(NSString *)roomId_{
	[[CMMScene sharedScene] openLoadingLayerWithTitle:@"Joining to room..."];
	[self _setUIEnable:NO];
	[[JGPhotonCloudClient defaultClient] joinRoomWithRoomName:roomId_];
}

-(void)jgPhotonCloudJoinedLobby:(JGPhotonCloudClient *)client_{
	CCLOG(@"PhotonCloudJoinedLobby!");
	[[CMMScene sharedScene] closeLoadingLayer];
	[self _loadRoomList];
}
-(void)jgPhotonCloudLeavedLobby:(JGPhotonCloudClient *)client_{
	CCLOG(@"PhotonCloudLeavedLobby!!");
	[self _setUIEnable:NO];
}

-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ joinedRoom:(NSString *)roomName_{
	CCLOG(@"PhotonCloud joined room! %d, %@",playerNr_,roomName_);
	[[CMMScene sharedScene] pushLayer:[TestRoom node]];
	[[CMMScene sharedScene] closeLoadingLayer];
}
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ failedToJoinRoom:(int)errorCode_ errorString:(NSString *)errorString_{
	[[CMMScene sharedScene] openPopupAlertWithContent:[NSString stringWithFormat:@"Failed to join room [%d]\n%@",errorCode_,errorString_]];
	[self _setUIEnable:YES];
	[[CMMScene sharedScene] closeLoadingLayer];
}
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ failedToCreateRoom:(int)errorCode_ errorString:(NSString *)errorString_{
	[[CMMScene sharedScene] openPopupAlertWithContent:[NSString stringWithFormat:@"Failed to create room [%d]\n%@",errorCode_,errorString_]];
	[self _setUIEnable:YES];
	[[CMMScene sharedScene] closeLoadingLayer];
}

// when create room, joined room automatically
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ createdRoom:(NSString *)roomName_{
	CCLOG(@"PhotonCloud createdRoom : %@",[[client_ currentlyRoom] CustomProperties]);
	[self jgPhotonCloud:client_ player:playerNr_ joinedRoom:roomName_];
}

-(void)cleanup{
	[[JGPhotonCloudClient defaultClient] setLobbyDelegate:nil];
	[super cleanup];
}

@end
