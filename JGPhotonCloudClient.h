//
//  JGPhotonCloudClient.h
//  ExitgamesCloudTest
//
//  Created by Kim Jazz on 13. 2. 9..
//  Copyright (c) 2013년 Kim Jazz. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "JGPhotonCloudConfig.h"
#import "JGPhotonCloudType.h"
#import "JGPhotonCloudUtil.h"

@class JGPhotonCloudClient;

typedef ExitGames::Common::DebugLevel::DebugLevel JGPhotonCloudClientDebugLevel;

#pragma mark - Delegate protocol

@protocol JGPhotonCloudCommonDelegate <NSObject>

-(void)jgPhotonCloudConnected:(JGPhotonCloudClient *)client_;
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ failedToConnect:(int)errorCode_ errorString:(NSString *)errorString_;

-(void)jgPhotonCloudDisconnected:(JGPhotonCloudClient *)client_;

-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ receivedConnectionError:(int)errorCode_;
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ errorReturn:(JGPhotonCloudErrorType::Type)errorType_ code:(int)code_;

@optional
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ debugReturn:(JGPhotonCloudClientDebugLevel)debugLevel_ string:(NSString *)string_;

-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ peerStateChanged:(JGPhotonCloudPeerState::State)peerState_ beforePeerState:(JGPhotonCloudPeerState::State)beforePeerState_;

@end

@protocol JGPhotonCloudLobbyDelegate <NSObject>

-(void)jgPhotonCloudJoinedLobby:(JGPhotonCloudClient *)client_;
-(void)jgPhotonCloudLeavedLobby:(JGPhotonCloudClient *)client_;

-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ joinedRoom:(NSString *)roomName_;
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ failedToJoinRoom:(int)errorCode_ errorString:(NSString *)errorString_;
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ failedToCreateRoom:(int)errorCode_ errorString:(NSString *)errorString_;

@optional
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ createdRoom:(NSString *)roomName_;

@end

@protocol JGPhotonCloudRoomDelegate <NSObject>

-(void)jgPhotonCloudLeavedRoom:(JGPhotonCloudClient *)client_;
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ failedToLeaveRoom:(int)errorCode_ errorString:(NSString *)errorString_;

-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ eventCode:(nByte)eventCode_ receivedEvent:(const NSDictionary *)event_;

@optional
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ whenPeerJoinedRoom:(NSString *)roomName_ playerData:(const EGLoadBalancingPlayer *)playerData_;
-(void)jgPhotonCloud:(JGPhotonCloudClient *)client_ player:(int)playerNr_ whenPeerLeavedRoom:(NSString *)roomName_;

@end

#pragma mark - Photon cloud client

@interface JGPhotonCloudClient : NSObject<EGLoadBalancingListener>{
	id<JGPhotonCloudCommonDelegate> commonDelegate;
	id<JGPhotonCloudLobbyDelegate> lobbyDelegate;
	id<JGPhotonCloudRoomDelegate> roomDelegate;
	
	JGPhotonCloudPeerState::State _lastPeerState;
}

+(JGPhotonCloudClient *)defaultClient;

-(void)initializeClientWithUserName:(NSString *)userName_;

@property (nonatomic, readonly) EGLoadBalancingClient *client;
@property (nonatomic, readonly) EGLoadBalancingMutablePlayer *currentlyPlayer;
@property (nonatomic, readonly) EGLoadBalancingMutableRoom *currentlyRoom;
@property (nonatomic, readonly) JGPhotonCloudPeerState::State peerState;
@property (nonatomic, readonly, getter = isClientInitialized) BOOL clientInitialized;
@property (nonatomic, readonly, getter = isInLobby) BOOL inLobby;
@property (nonatomic, readonly, getter = isInRoom) BOOL inRoom;
@property (nonatomic, retain) id<JGPhotonCloudCommonDelegate> commonDelegate;
@property (nonatomic, retain) id<JGPhotonCloudLobbyDelegate> lobbyDelegate;
@property (nonatomic, retain) id<JGPhotonCloudRoomDelegate> roomDelegate;

@end

@interface JGPhotonCloudClient(Common)

-(BOOL)connectServer;

@end

@interface JGPhotonCloudClient(Lobby)

-(BOOL)joinLobby;
-(BOOL)leaveLobby;

-(BOOL)joinRoomWithRoomName:(NSString *)roomName_;
-(BOOL)joinRandomRoom;

-(BOOL)createRoomWithMaxPlayers:(uint)maxPlayers_ isVisible:(BOOL)isVisible_ isOpen:(BOOL)isOpen_ customProperties:(NSDictionary *)customProperties_ showPropertyKeys:(NSArray *)showPropertyKeys_;
-(BOOL)createRoomWithMaxPlayers:(uint)maxPlayers_ isVisible:(BOOL)isVisible_ isOpen:(BOOL)isOpen_ customProperties:(NSDictionary *)customProperties_;
-(BOOL)createRoomWithMaxPlayers:(uint)maxPlayers_ isVisible:(BOOL)isVisible_ isOpen:(BOOL)isOpen_;
-(BOOL)createRoomWithMaxPlayers:(uint)maxPlayers_ isVisible:(BOOL)isVisible_;
-(BOOL)createRoomWithMaxPlayers:(uint)maxPlayers_;

-(NSDictionary *)roomList;

@end

@interface JGPhotonCloudClient(Room)

-(BOOL)leaveRoom;

-(BOOL)sendEventTo:(int)playerNr_ eventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_ channelId:(nByte)channelId_;
-(BOOL)sendEventTo:(int)playerNr_ eventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_;
-(BOOL)sendEventTo:(int)playerNr_ eventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_;
-(BOOL)sendEventTo:(int)playerNr_ eventCode:(nByte)eventCode_;

-(BOOL)sendEventToAllWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_ channelId:(nByte)channelId_;
-(BOOL)sendEventToAllWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_;
-(BOOL)sendEventToAllWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_;
-(BOOL)sendEventToAllWithEventCode:(nByte)eventCode_;

@end

@interface JGPhotonCloudClient(Deprecated)

-(BOOL)sendEventWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_ channelId:(nByte)channelId_ DEPRECATED_ATTRIBUTE;
-(BOOL)sendEventWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ isReliable:(BOOL)isReliable_ DEPRECATED_ATTRIBUTE;
-(BOOL)sendEventWithEventCode:(nByte)eventCode_ parameters:(NSDictionary *)parameters_ DEPRECATED_ATTRIBUTE;
-(BOOL)sendEventWithEventCode:(nByte)eventCode_ DEPRECATED_ATTRIBUTE;

@end