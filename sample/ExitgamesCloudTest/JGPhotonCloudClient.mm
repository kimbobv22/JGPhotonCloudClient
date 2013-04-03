//
//  JGPhotonCloudClient.mm
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 9..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#import "JGPhotonCloudClient.h"

static JGPhotonCloudClient *_sharedJGPhotonCloudClient_ = nil;

@interface JGPhotonCloudClient(Private)

-(void)_whenChangedPeerStateTo:(JGPhotonCloudPeerState::State)peerState_;

@end

@implementation JGPhotonCloudClient(Private)

-(void)_whenChangedPeerStateTo:(JGPhotonCloudPeerState::State)peerState_{
	if(_lastPeerState == peerState_) return;
	if(jgFuncRespondsToSelector(commonDelegate, @selector(jgPhotonCloud:peerStateChanged:beforePeerState:))){
		[commonDelegate jgPhotonCloud:self peerStateChanged:peerState_ beforePeerState:_lastPeerState];
	}
	
	_lastPeerState = peerState_;
}

@end

@implementation JGPhotonCloudClient{
	EGLoadBalancingClient *_client;
}
@synthesize client = _client,currentlyPlayer,currentlyRoom;
@synthesize peerState,clientInitialized,inLobby,inRoom;
@synthesize commonDelegate,lobbyDelegate,roomDelegate;

+(JGPhotonCloudClient *)defaultClient{
	if(!_sharedJGPhotonCloudClient_){
		_sharedJGPhotonCloudClient_ = [[JGPhotonCloudClient alloc] init];
	}

	return _sharedJGPhotonCloudClient_;
}

-(id)init{
	if(!(self = [super init])) return self;

	_lastPeerState = JGPhotonCloudPeerState::Uninitialized;
	
	commonDelegate = nil;
	lobbyDelegate = nil;
	roomDelegate = nil;

	return self;
}

-(EGLoadBalancingMutablePlayer *)currentlyPlayer{
	if([self peerState] < JGPhotonCloudPeerState::PeerCreated) return nil;
	return [[self currentlyRoom] LocalPlayer];
}
-(EGLoadBalancingMutableRoom *)currentlyRoom{
	if(![self isInRoom]) return nil;
	return [_client CurrentlyJoinedRoom];
}

-(JGPhotonCloudPeerState::State)peerState{
	return (JGPhotonCloudPeerState::State)[_client State];
}
-(BOOL)isClientInitialized{
	return (_client != nil && [self peerState] != JGPhotonCloudPeerState::Uninitialized);
}
-(BOOL)isInLobby{
	return [_client IsInLobby];
}
-(BOOL)isInRoom{
	return [_client IsInRoom];
}

-(void)initializeClientWithUserName:(NSString *)userName_{
	if([self isClientInitialized]) return;
	[EGBase setListener:self];
	_client = [[EGLoadBalancingClient alloc] initClient:self :jgVarJGPhotonCloudConfig_appID :jgVarJGPhotonCloudConfig_version :userName_ :jgVarJGPhotonCloudConfig_usingTcp];
	[_client setAutoJoinLobby:NO];
	[_client setDebugOutputLevel:jgVarJGPhotonCloudConfig_debugLevel];
	[NSTimer scheduledTimerWithTimeInterval:jgVarJGPhotonCloudConfig_serviceTimeInterval target:self selector:@selector(_update:) userInfo:nil repeats:true];
}

-(void)_update:(NSTimeInterval)interval_{
	[_client service:true];
	[self _whenChangedPeerStateTo:[self peerState]];
}

#pragma mark - delegate method(Common)

// implement your error-handling here
- (void) connectionErrorReturn:(int)errorCode{
	if(!commonDelegate) return;
	[commonDelegate jgPhotonCloud:self receivedConnectionError:errorCode];
}
- (void) connectReturn:(int)errorCode :(const NSString* const)errorString{
	if(!commonDelegate) return;

	if(errorCode == 0){
		[commonDelegate jgPhotonCloudConnected:self];
	}else{
		[commonDelegate jgPhotonCloud:self failedToConnect:errorCode errorString:[errorString toString]];
	}
}
- (void) disconnectReturn{
	if(!commonDelegate) return;
	[commonDelegate jgPhotonCloudDisconnected:self];
}

- (void)debugReturn:(NSString*)string{
	if(!commonDelegate) return;
	
	[commonDelegate jgPhotonCloud:self debugReturn:ExitGames::Common::DebugLevel::ALL string:string];
}
- (void)debugReturn:(ExitGames::Common::DebugLevel::DebugLevel)debugLevel :(const NSString* const)string{
	if(!jgFuncRespondsToSelector(commonDelegate, @selector(jgPhotonCloud:debugReturn:string:))) return;
	[commonDelegate jgPhotonCloud:self debugReturn:debugLevel string:[string toString]];
}

- (void) clientErrorReturn:(int)errorCode{
	if(!commonDelegate) return;
	[commonDelegate jgPhotonCloud:self errorReturn:JGPhotonCloudErrorType::ClientError code:errorCode];
}
- (void) warningReturn:(int)warningCode{
	if(!commonDelegate) return;
	[commonDelegate jgPhotonCloud:self errorReturn:JGPhotonCloudErrorType::Warning code:warningCode];
}
- (void) serverErrorReturn:(int)errorCode{
	if(!commonDelegate) return;
	[commonDelegate jgPhotonCloud:self errorReturn:JGPhotonCloudErrorType::ServerError code:errorCode];
}

- (void) joinLobbyReturn{
	if(!lobbyDelegate) return;
	[lobbyDelegate jgPhotonCloudJoinedLobby:self];
}

#pragma mark - delegate method(Lobby)

- (void) leaveLobbyReturn{
	if(!lobbyDelegate) return;
	[lobbyDelegate jgPhotonCloudLeavedLobby:self];
}

-(void)_joinRoomReturn:(int)localPlayerNr :(const NSDictionary* const)roomProperties :(const NSDictionary* const)playerProperties :(int)errorCode :(const NSString* const)errorString{
	if(!lobbyDelegate) return;
	if(errorCode == 0){
		[lobbyDelegate jgPhotonCloud:self player:localPlayerNr joinedRoom:[[_client CurrentlyJoinedRoom] Name]];
	}else{
		[lobbyDelegate jgPhotonCloud:self player:localPlayerNr failedToJoinRoom:errorCode errorString:[errorString toString]];
	}
}
- (void) joinRoomReturn:(int)localPlayerNr :(const NSDictionary* const)roomProperties :(const NSDictionary* const)playerProperties :(int)errorCode :(const NSString* const)errorString{
	[self _joinRoomReturn:localPlayerNr :roomProperties :playerProperties :errorCode :errorString];
}
- (void) joinRandomRoomReturn:(int)localPlayerNr :(const NSDictionary* const)roomProperties :(const NSDictionary* const)playerProperties :(int)errorCode :(const NSString* const)errorString{
	[self _joinRoomReturn:localPlayerNr :roomProperties :playerProperties :errorCode :errorString];
}

- (void) createRoomReturn:(int)localPlayerNr :(const NSDictionary* const)roomProperties :(const NSDictionary* const)playerProperties :(int)errorCode :(const NSString* const)errorString{
	if(!lobbyDelegate) return;
	if(errorCode == 0){
		if(jgFuncRespondsToSelector(lobbyDelegate, @selector(jgPhotonCloud:player:createdRoom:))){
			[lobbyDelegate jgPhotonCloud:self player:localPlayerNr createdRoom:[[_client CurrentlyJoinedRoom] Name]];
		}
	}else{
		[lobbyDelegate jgPhotonCloud:self player:localPlayerNr failedToCreateRoom:errorCode errorString:[errorString toString]];
	}
}

#pragma mark - delegate method(Room)

- (void) leaveRoomReturn:(int)errorCode :(const NSString* const)errorString{
	if(!roomDelegate) return;
	
	if(errorCode == 0){
		[roomDelegate jgPhotonCloudLeavedRoom:self];
	}else{
		[roomDelegate jgPhotonCloud:self failedToLeaveRoom:errorCode errorString:[errorString toString]];
	}
}

- (void) joinRoomEventAction:(int)playerNr :(const EGArray* const)playernrs :(const EGLoadBalancingPlayer* const)player{
	if(!jgFuncRespondsToSelector(roomDelegate, @selector(jgPhotonCloud:player:whenPeerJoinedRoom:playerData:))) return;
	[roomDelegate jgPhotonCloud:self player:playerNr whenPeerJoinedRoom:[[_client CurrentlyJoinedRoom] Name] playerData:player];
}
- (void) leaveRoomEventAction:(int)playerNr{
	if(!jgFuncRespondsToSelector(roomDelegate, @selector(jgPhotonCloud:player:whenPeerLeavedRoom:))) return;
	[roomDelegate jgPhotonCloud:self player:playerNr whenPeerLeavedRoom:[[_client CurrentlyJoinedRoom] Name]];
}

- (void) customEventAction:(int)playerNr :(nByte)eventCode :(const NSDictionary* const)eventContent{
	if(!roomDelegate) return;
	[roomDelegate jgPhotonCloud:self player:playerNr eventCode:eventCode receivedEvent:eventContent];
}

#pragma mark - etc

- (void) gotQueuedReturn{
	NSLog(@"gotQueuedReturn");
}

#pragma mark -

-(void)dealloc{
	[_client release];
	[commonDelegate release];
	[lobbyDelegate release];
	[roomDelegate release];
	[super dealloc];
}

@end

@implementation JGPhotonCloudClient(Common)

-(BOOL)connectServer{
	return [_client connect];
}

@end

@implementation JGPhotonCloudClient(Lobby)

-(BOOL)joinLobby{
	return [_client opJoinLobby];
}
-(BOOL)leaveLobby{
	return [_client opLeaveLobby];
}

-(BOOL)joinRoomWithRoomName:(NSString *)roomName_{
	return [_client opJoinRoom:roomName_];
}
-(BOOL)joinRandomRoom{
	return [_client opJoinRandomRoom];
}

-(BOOL)createRoomWithMaxPlayers:(uint)maxPlayers_ isVisible:(BOOL)isVisible_ isOpen:(BOOL)isOpen_ customProperties:(NSDictionary *)customProperties_ showPropertyKeys:(NSArray *)showPropertyKeys_{	
	return [_client opCreateRoom:nil :isVisible_ :isOpen_ :maxPlayers_ :customProperties_ :[NSDictionary dictionary] :[EGArray arrayWithArray:showPropertyKeys_]];
}
-(BOOL)createRoomWithMaxPlayers:(uint)maxPlayers_ isVisible:(BOOL)isVisible_ isOpen:(BOOL)isOpen_ customProperties:(NSDictionary *)customProperties_{
	return [self createRoomWithMaxPlayers:maxPlayers_ isVisible:isVisible_ isOpen:isOpen_ customProperties:customProperties_ showPropertyKeys:[customProperties_ allKeys]];
}
-(BOOL)createRoomWithMaxPlayers:(uint)maxPlayers_ isVisible:(BOOL)isVisible_ isOpen:(BOOL)isOpen_{
	return [self createRoomWithMaxPlayers:maxPlayers_ isVisible:isVisible_ isOpen:isVisible_ customProperties:[NSDictionary dictionary]];
}
-(BOOL)createRoomWithMaxPlayers:(uint)maxPlayers_ isVisible:(BOOL)isVisible_{
	return [self createRoomWithMaxPlayers:maxPlayers_ isVisible:isVisible_ isOpen:YES];
}
-(BOOL)createRoomWithMaxPlayers:(uint)maxPlayers_{
	return [self createRoomWithMaxPlayers:maxPlayers_ isVisible:YES];
}

-(NSDictionary *)roomList{
	return [_client RoomList];
}

@end

@implementation JGPhotonCloudClient(Room)

-(BOOL)leaveRoom{
	return [_client opLeaveRoom];
}

-(BOOL)_sendEventTo:(int *)playerNr_ playerNrCount:(short)playerNrCount_ eventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_ channelId:(nByte)channelId_{
	return [_client opRaiseEvent:isReliable_ :parameters_ :eventCode_ :channelId_ :playerNr_ :playerNrCount_];
}

-(BOOL)sendEventTo:(int)playerNr_ eventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_ channelId:(nByte)channelId_{
	int *playerNrs_ = (int *)malloc(sizeof(int));
	playerNrs_[0] = playerNr_;	
	BOOL result_ = [self _sendEventTo:playerNrs_ playerNrCount:1 eventCode:eventCode_ parameters:parameters_ isReliable:isReliable_ channelId:channelId_];
	free(playerNrs_);
	
	return result_;
}
-(BOOL)sendEventTo:(int)playerNr_ eventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_{
	return [self sendEventTo:playerNr_ eventCode:eventCode_ parameters:parameters_ isReliable:isReliable_ channelId:0];
}
-(BOOL)sendEventTo:(int)playerNr_ eventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_{
	return [self sendEventTo:playerNr_ eventCode:eventCode_ parameters:parameters_ isReliable:YES];
}
-(BOOL)sendEventTo:(int)playerNr_ eventCode:(nByte)eventCode_{
	return [self sendEventTo:playerNr_ eventCode:eventCode_ parameters:nil];
}

-(BOOL)sendEventToAllWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_ channelId:(nByte)channelId_{
	EGArray *players_ = [[_client CurrentlyJoinedRoom] Players];
	uint playersCount_ = [players_ count];
	int *playerNrs_ = (int *)malloc(sizeof(int)*playersCount_);
	
	for(uint index_=0;index_<playersCount_;++index_){
		playerNrs_[index_] = [[players_ objectAtIndex:index_] Number];
	}
	BOOL result_ = [self _sendEventTo:playerNrs_ playerNrCount:playersCount_ eventCode:eventCode_ parameters:parameters_ isReliable:isReliable_ channelId:channelId_];
	free(playerNrs_);
	
	return result_;
}
-(BOOL)sendEventToAllWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_{
	return [self sendEventToAllWithEventCode:eventCode_ parameters:parameters_ isReliable:isReliable_ channelId:0];
}
-(BOOL)sendEventToAllWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_{
	return [self sendEventToAllWithEventCode:eventCode_ parameters:parameters_ isReliable:YES];
}
-(BOOL)sendEventToAllWithEventCode:(nByte)eventCode_{
	return [self sendEventToAllWithEventCode:eventCode_ parameters:nil];
}

@end

@implementation JGPhotonCloudClient(Deprecated)

-(BOOL)sendEventWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_ channelId:(nByte)channelId_{
	return [self sendEventToAllWithEventCode:eventCode_ parameters:parameters_ isReliable:isReliable_ channelId:channelId_];
}
-(BOOL)sendEventWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_{
	return [self sendEventToAllWithEventCode:eventCode_ parameters:parameters_ isReliable:isReliable_];
}
-(BOOL)sendEventWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_{
	return [self sendEventToAllWithEventCode:eventCode_ parameters:parameters_];
}
-(BOOL)sendEventWithEventCode:(nByte)eventCode_{
	return [self sendEventToAllWithEventCode:eventCode_];
}

@end