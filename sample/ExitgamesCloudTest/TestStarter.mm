#import "TestStarter.h"
#import "TestLobby.h"

@implementation TestStarter

-(id)initWithColor:(ccColor4B)color width:(GLfloat)w height:(GLfloat)h{
	if(!(self = [super initWithColor:color width:w height:h])) return self;
	
	[[CMMDrawingManager sharedManager] addDrawingItemWithFileName:@"barFrame000"];
	
	textFieldUserName = [CMMControlItemText controlItemTextWithFrameSeq:0 width:[self contentSize].width*0.5f];
	[textFieldUserName setTitle:@"User name"];
	[textFieldUserName setPlaceHolder:@"enter User name"];
	[textFieldUserName setPosition:cmmFuncCommon_positionInParent(self, textFieldUserName,ccp(0.35f,0.55f))];
	
	btnConnect = [CMMMenuItemL menuItemWithFrameSeq:0 batchBarSeq:0];
	[btnConnect setTitle:@"Connect"];
	[btnConnect setCallback_pushup:^(id item_) {
		if([[textFieldUserName itemValue] length] == 0){
			[[CMMScene sharedScene] openPopupAlertWithContent:@"User name should not be nil" closeBlock:^(SimplePopupAlert *alert_) {
				[textFieldUserName showTextField];
			}];
			return;
		}
		
		[self _connectServer];
	}];
	
	[btnConnect setPosition:cmmFuncCommon_positionFromOtherNode(textFieldUserName, btnConnect, ccp(1.0f,0.0f),ccp(10.0f,0.0f))];
	
	[self addChild:textFieldUserName];
	[self addChild:btnConnect];
	
	[[JGPhotonCloudClient defaultClient] setCommonDelegate:self];
	
	return self;
}

-(void)sceneDidEndTransition:(CMMScene *)scene_{
	[textFieldUserName showTextField];
}

-(void)_settupPhotonClientWithUserName:(NSString *)userName_{
	JGPhotonCloudClient *pCloudClient_ = [JGPhotonCloudClient defaultClient];
	if([pCloudClient_ isClientInitialized]) return;
	
	[pCloudClient_ initializeClientWithUserName:userName_];
}
-(void)_connectServer{
	[[CMMScene sharedScene] openLoadingLayerWithTitle:@"Connecting to server..."];
	[btnConnect setEnable:NO];
	
	[self _settupPhotonClientWithUserName:[textFieldUserName itemValue]];
	[[JGPhotonCloudClient defaultClient] connectServer];
}

-(void)jgPhotonCloudConnected:(JGPhotonCloudClient *)client_{
	CCLOG(@"PhotonCloudConnected! try to join lobby");
	[[CMMScene sharedScene] openLoadingLayerWithTitle:@"Joining to lobby..."];
	[[JGPhotonCloudClient defaultClient] joinLobby];
}
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ failedToConnect:(int)errorCode_ errorString:(NSString *)errorString_{
	[[CMMScene sharedScene] openPopupAlertWithContent:[NSString stringWithFormat:@"failed to connect errorCode : %d\n%@",errorCode_,errorString_]];
	[btnConnect setEnable:YES];
}

-(void)jgPhotonCloudDisconnected:(JGPhotonCloudClient *)client_{
	CCLOG(@"PhotonCloudDisconnected! return to starter");
	[[CMMScene sharedScene] pushStaticLayerItemAtKey:varTestStarter_staticKey];
}

-(void)jgPhotonCloudJoinedLobby:(JGPhotonCloudClient *)client_{
	CCLOG(@"PhotonCloudJoinedLobby! go to lobby");
	[[CMMScene sharedScene] closeLoadingLayer];
	[[CMMScene sharedScene] pushStaticLayerItemAtKey:varTestLobby_staticKey];
}

-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ receivedConnectionError:(int)errorCode_{
	[[CMMScene sharedScene] openPopupAlertWithContent:[NSString stringWithFormat:@"connection error! [%d]",errorCode_]];
	[btnConnect setEnable:YES];
}
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ errorReturn:(JGPhotonCloudErrorType::Type)errorType_ code:(int)code_{
	CCLOG(@"errorReturn! [%d - %d]",errorType_,code_);
}

-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ debugReturn:(JGPhotonCloudClientDebugLevel)debugLevel_ string:(NSString *)string_{
	CCLOG(@"[DEBUG lv.%d] %@",debugLevel_,string_);
}

-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ peerStateChanged:(JGPhotonCloudPeerState::State)peerState_ beforePeerState:(JGPhotonCloudPeerState::State)beforePeerState_{
	CCLOG(@"peerStateChanged! %d -> %d",beforePeerState_,peerState_);
}

-(void)cleanup{
	[[JGPhotonCloudClient defaultClient] setCommonDelegate:nil];
	[super cleanup];
}

@end