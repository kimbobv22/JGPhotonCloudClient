/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2012 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#import "EventCode.h"
#import "JoinType.h"
#import "OperationCode.h"
#import "ParameterCode.h"
#import "LoadBalancingPlayer+Internal.h"
#import "LoadBalancingRoom+Internal.h"
#import "LoadBalancingPeer+Internal.h"
#import "LoadBalancingClient.h"
#import <Foundation/NSException.h>

/** @file LoadBalancingClient.h */

namespace LoadBalancing
{
	// default master server address and port
	const NSString* const MASTERSERVER = @"app.exitgamescloud.com";
	const NSString* const UDP_PORT = @":5055";
	const NSString* const TCP_PORT = @":4530";
	
	const bool SEND_AUTHENTICATE_ENCRYPTED = true;
}

using namespace LoadBalancing;

@interface LoadBalancingClient (Private)

@property (readonly) bool IsOnGameServer;

- (void) readoutProperties:(NSMutableDictionary**)pRoomProperties :(NSMutableDictionary**)pPlayerProperties :(bool)multiplePlayers :(int)targetPlayerNr;
- (void) handleConnectionFlowError:(LoadBalancing::PeerStates::PeerStates)oldState :(int)errorCode :(const NSString* const)errorString;
- (void) onConnectToMasterFinished:(bool)comingFromGameserver;

@end



@implementation LoadBalancingClient

// properties

@synthesize Listener = mListener;
@synthesize State = mState;

- (const NSString*) MasterserverAddress
{
	return mMasterserver;
}

@synthesize CountPlayersIngame = mPeerCount;
@synthesize CountGamesRunning = mRoomCount;

- (int) CountPlayersOnline
{
	return mPeerCount + mMasterPeerCount;
}

@synthesize QueuePosition = mQueuePosition;
@synthesize RoomList = mRoomList;
@synthesize CurrentlyJoinedRoom = mCurrentlyJoinedRoom;

- (bool) IsInRoom
{
	return self.IsInGameRoom || self.IsInLobby;
}

- (bool) IsInGameRoom
{
	return mState == PeerStates::Joined;
}

- (bool) IsInLobby
{
	return mState == PeerStates::JoinedLobby;
}

@synthesize AutoJoinLobby = mAutoJoinLobby;



// methods

- (id) initWithPhotonListener:(id<PhotonListener>)listener
{
	DEBUG_ONLY(NSAssert(false, @"LoadBalancingClient::initWithPhotonListener(): not implemented!"));
	return nil;
}

- (id) initWithPhotonListener:(id<PhotonListener>)listener :(bool)useTcp
{
	DEBUG_ONLY(NSAssert(false, @"LoadBalancingClient::initWithPhotonListener(): not implemented!"));
	return nil;
}

- (id) initClient:(id<LoadBalancingListener>)listener :(NSString*)applicationID :(NSString*)appVersion :(NSString*)username
{
	return [self initClient:listener :applicationID :appVersion :username :false];
}

- (id) initClient:(id<LoadBalancingListener>)listener :(NSString*)applicationID :(NSString*)appVersion :(NSString*)username :(bool)useTcp
{
	self = [super initWithPhotonListener:self :mUseTcp=useTcp];
	mListener = listener;
	mState = PeerStates::PeerCreated;
	mAppVersion = [appVersion retain];
	mCurrentlyJoinedRoom = [[LoadBalancingMutableRoom alloc] initLoadBalancingRoom:@"" :nil :nil :nil];
	[mCurrentlyJoinedRoom setLocalPlayer:-1 :[NSDictionary dictionaryWithObject:username forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]]];
	mAppID = [applicationID retain];
	mAutoJoinLobby = true;
	return self;
}

- (bool) connect
{
	return [self connect:[MASTERSERVER stringByAppendingString:(NSString*)(mUseTcp?TCP_PORT:UDP_PORT)]];
}

- (bool) connect:(const NSString* const)masterServerAddress
{
	mState = PeerStates::Connecting;
	return [super connect:mMasterserver=[masterServerAddress copy]];
}

- (void) disconnect
{
	mState = PeerStates::Disconnecting;
	[super disconnect];
}

- (bool) opJoinLobby
{
	if(self.IsInRoom)
	{
		PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"already in a room");
		return false;
	}
	mLastJoinType = JoinType::EXPLICIT_JOIN_LOBBY;
	return [super opJoinLobby];
}

- (bool) opLeaveLobby
{
	if(!self.IsInLobby)
	{
		PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"lobby isn't currently joined");
		return false;
	}
	return [super opLeaveLobby];
}

- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(NSDictionary*)customLocalPlayerProperties :(EGArray*)propsListedInLobby
{
	if(self.IsInGameRoom)
	{
		PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"already in a gameroom");
		return false;
	}
	if(mRoomName != gameID)
	{
		[mRoomName release];
		mRoomName = [gameID retain];
	}

	NSMutableDictionary* op = [self opCreateRoomImplementation:gameID :isVisible :isOpen :maxPlayers :self.IsOnGameServer?customRoomProperties:nil :self.IsOnGameServer?customLocalPlayerProperties:nil :self.IsOnGameServer?propsListedInLobby:nil];
	if(mCurrentlyJoinedRoom.LocalPlayer.Name)
	{
		if([op objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]])
			[[op objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]] setObject:mCurrentlyJoinedRoom.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]];
		else
			[op setObject:[NSDictionary dictionaryWithObject:mCurrentlyJoinedRoom.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]] forKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]];
	}
	if(![self opCustom:[OperationRequest operationRequestWithOperationCode:OperationCode::CREATE_ROOM :op] :true])
		return false;

	NSMutableDictionary* roomProps = [LoadBalancingPeer stripToCustomProperties:customRoomProperties];
	[roomProps setObject:[NSValue value:&isOpen withObjCType:@encode(bool)] forKey:[NSValue value:&Properties::Room::IS_OPEN withObjCType:@encode(nByte)]];
	[roomProps setObject:[NSValue value:&isVisible withObjCType:@encode(bool)] forKey:[NSValue value:&Properties::Room::IS_VISIBLE withObjCType:@encode(nByte)]];
	[roomProps setObject:[NSValue value:&maxPlayers withObjCType:@encode(nByte)] forKey:[NSValue value:&Properties::Room::MAX_PLAYERS withObjCType:@encode(nByte)]];
	[roomProps setObject:[propsListedInLobby.Type isEqual:NSStringFromClass([NSString class])]?[[propsListedInLobby copyDeep] autorelease]:[EGArray arrayWithType:NSStringFromClass([NSString class])] forKey:[NSValue value:&Properties::Room::PROPS_LISTED_IN_LOBBY withObjCType:@encode(nByte)]];

	LoadBalancingMutablePlayer* localPlayer = [[mCurrentlyJoinedRoom.LocalPlayer retain] autorelease];
	[mCurrentlyJoinedRoom release];
	mCurrentlyJoinedRoom = [[LoadBalancingMutableRoom alloc] initLoadBalancingRoom:gameID :roomProps :self :propsListedInLobby];
	mCurrentlyJoinedRoom.LocalPlayer = localPlayer;
	[mCurrentlyJoinedRoom.LocalPlayer cacheProperties:[LoadBalancingPeer stripToCustomProperties:customLocalPlayerProperties]];
	return true;
}

- (bool) opJoinRoom:(NSString*)gameID :(NSDictionary*)customLocalPlayerProperties
{
	if(self.IsInGameRoom)
	{
		PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"already in a gameroom");
		return false;
	}
	if(mRoomName != gameID)
	{
		[mRoomName release];
		mRoomName = [gameID retain];
	}
	NSMutableDictionary* op = [self opJoinRoomImplementation:gameID :self.IsOnGameServer?customLocalPlayerProperties:nil];
	if(mCurrentlyJoinedRoom.LocalPlayer.Name)
	{
		if([op objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]])
			[[op objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]] setObject:mCurrentlyJoinedRoom.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]];
		else
			[op setObject:[NSDictionary dictionaryWithObject:mCurrentlyJoinedRoom.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]] forKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]];
	}
	if(!gameID || ![super opCustom:[OperationRequest operationRequestWithOperationCode:OperationCode::JOIN_ROOM :op] :true])
		return false;
	LoadBalancingMutablePlayer* localPlayer = [[mCurrentlyJoinedRoom.LocalPlayer retain] autorelease];
	[mCurrentlyJoinedRoom release];
	mCurrentlyJoinedRoom = [[LoadBalancingMutableRoom alloc] initLoadBalancingRoom:gameID :nil :self :nil];
	mCurrentlyJoinedRoom.LocalPlayer = localPlayer;
	[mCurrentlyJoinedRoom.LocalPlayer cacheProperties:[LoadBalancingPeer stripToCustomProperties:customLocalPlayerProperties]];
	return true;
}

- (bool) opJoinRandomRoom:(NSDictionary*)customRoomProperties :(nByte)maxPlayers
{
	return [self opJoinRandomRoom:customRoomProperties :maxPlayers :nil];
}

- (bool) opJoinRandomRoom:(NSDictionary*)customRoomProperties :(nByte)maxPlayers :(NSDictionary*)customLocalPlayerProperties
{
	if(self.IsInGameRoom)
	{
		PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"already in a gameroom");
		return false;
	}
	if(![super opJoinRandomRoom:customRoomProperties :maxPlayers])
		return false;

	LoadBalancingMutablePlayer* localPlayer = [[mCurrentlyJoinedRoom.LocalPlayer retain] autorelease];
	[mCurrentlyJoinedRoom release];
	mCurrentlyJoinedRoom = [[LoadBalancingMutableRoom alloc] initLoadBalancingRoom:@"" :[LoadBalancingPeer stripToCustomProperties:customRoomProperties] :self :nil];
	[localPlayer cleanCustomProperties];
	[localPlayer cacheProperties:customLocalPlayerProperties];
	mCurrentlyJoinedRoom.LocalPlayer = localPlayer;
	return true;
}


- (bool) opLeaveRoom
{
	if(!self.IsInGameRoom)
	{
		PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"no gameroom is currently joined");
		return false;
	}
	if(![super opLeaveRoom])
		return false;
	mState = PeerStates::Leaving;
	return true;
}

- (bool) opAuthenticate:(NSString*)appID :(NSString*)appVersion :(bool)encrypted
{
	NSAssert(false, @"opAuthenticate() is not part of the public interface of class LoadBalancingClient!");
	return false;
}



// inherited

- (void) dealloc
{
	[mCurrentlyJoinedRoom release];
	[mRoomList release];
	[mAppID release];
	[mRoomName release];
	[mAppVersion release];
	[mGameserver release];
	[super dealloc];
}



// protocol implementations

- (void) onOperationResponse:(OperationResponse*)operationResponse
{
	PhotonPeer_sendDebugOutput(self, operationResponse.ReturnCode?DEBUG_LEVEL_ERRORS:DEBUG_LEVEL_INFO, [operationResponse toString:true :true]);
	switch(operationResponse.OperationCode)
	{
		case OperationCode::AUTHENTICATE:
		{
			enum PeerStates::PeerStates oldState = mState;
			if(operationResponse.ReturnCode)
			{
				PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"authentication failed with errorcode %d: %ls", operationResponse.ReturnCode, operationResponse.DebugMessage.UTF32String);
				[self handleConnectionFlowError:oldState :operationResponse.ReturnCode :operationResponse.DebugMessage];
				break;
			}
			else
			{
				if(mState == PeerStates::Connected || mState == PeerStates::ConnectedComingFromGameserver)
				{
					[[operationResponse objectForKey:[NSValue value:&ParameterCode::POSITION withObjCType:@encode(nByte)]] getValue:&mQueuePosition];
					if(mQueuePosition) // should only happen, if just out of nowhere the amount of players going online at the same time is increasing faster, than automatically started additional gameservers could have been booten up
					{
						mState = mState==PeerStates::Connected?PeerStates::Queued:PeerStates::QueuedComingFromGameserver;
						[mListener gotQueuedReturn];
						break;
					}
					mState = mState==PeerStates::Connected?PeerStates::Authenticated:PeerStates::AuthenticatedComingFromGameserver;
					if(mAutoJoinLobby)
					{
						[self opJoinLobby];
						mLastJoinType = JoinType::AUTO_JOIN_LOBBY;
					}
					else
						[self onConnectToMasterFinished:oldState==PeerStates::ConnectedComingFromGameserver];
					
				}
				else if(mState == PeerStates::ConnectedToGameserver)
				{
					mState = PeerStates::Joining;
					if(mLastJoinType == JoinType::CREATE_ROOM)
						[self opCreateRoom:mRoomName :mCurrentlyJoinedRoom.IsVisible :mCurrentlyJoinedRoom.IsOpen :mCurrentlyJoinedRoom.MaxPlayers :mCurrentlyJoinedRoom.CustomProperties :mCurrentlyJoinedRoom.LocalPlayer.CustomProperties :mCurrentlyJoinedRoom.PropsListedInLobby];
					else
						[self opJoinRoom:mRoomName :mCurrentlyJoinedRoom.LocalPlayer.CustomProperties];
				}
			}
		}
			break;
		case OperationCode::CREATE_ROOM:
		case OperationCode::JOIN_ROOM:
			if(self.IsOnGameServer)
			{
				if(operationResponse.ReturnCode)
				{
					PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"%ls failed with errorcode %d: %ls. Client is therefore returning to masterserver!", operationResponse.OperationCode==OperationCode::CREATE_ROOM?L"opCreateRoom":L"opJoinRoom", operationResponse.ReturnCode, operationResponse.DebugMessage);
					[self handleConnectionFlowError:mState :operationResponse.ReturnCode :operationResponse.DebugMessage];
					break;
				}
				mState = PeerStates::Joined;
				int nr;
				[[operationResponse objectForKey:[NSValue value:&ParameterCode::PLAYERNR withObjCType:@encode(nByte)]] getValue:&nr];
				[mCurrentlyJoinedRoom setLocalPlayer:nr :[NSDictionary dictionaryWithObject:mCurrentlyJoinedRoom.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]]];
				mCurrentlyJoinedRoom.Players = [EGMutableArray arrayWithType:NSStringFromClass([LoadBalancingPlayer class])];
				NSMutableDictionary* roomProperties = [NSMutableDictionary dictionaryWithDictionary:[operationResponse objectForKey:[NSValue value:&ParameterCode::ROOM_PROPERTIES withObjCType:@encode(nByte)]]];
				NSMutableDictionary* playerProperties = [NSMutableDictionary dictionaryWithDictionary:[operationResponse objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]]];
				NSValue** numbers = (id*)MALLOC(playerProperties.count*sizeof(NSValue*));
				NSDictionary** vals = (NSDictionary**)MALLOC(playerProperties.count*sizeof(NSString*));
				[playerProperties getObjects:vals andKeys:numbers];
				for(int i=0; i<playerProperties.count; i++)
				{
					[numbers[i] getValue:&nr];
					[mCurrentlyJoinedRoom addPlayer:nr :vals[i]];
				}
				FREE(numbers);
				FREE(vals);
				[self readoutProperties:&roomProperties :&playerProperties :true :0];
				switch(mLastJoinType)
				{
					case JoinType::CREATE_ROOM:
						[mListener createRoomReturn:nr :roomProperties :playerProperties :operationResponse.ReturnCode :operationResponse.DebugMessage];
						break;
					case JoinType::JOIN_ROOM:
						[mListener joinRoomReturn:nr :roomProperties :playerProperties :operationResponse.ReturnCode :operationResponse.DebugMessage];
						break;
					case JoinType::JOIN_RANDOM_ROOM:
						[mListener joinRandomRoomReturn:nr :roomProperties :playerProperties :operationResponse.ReturnCode :operationResponse.DebugMessage];
						break;
					default:
						break;
				}
				break;
			}
			else
			{
				switch(operationResponse.OperationCode)
				{
					case OperationCode::CREATE_ROOM:
					{
						if(operationResponse.ReturnCode)
						{
							PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"opCreateRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.ReturnCode, operationResponse.DebugMessage.UTF32String);
							[mListener createRoomReturn:0 :nil :nil :operationResponse.ReturnCode :operationResponse.DebugMessage];
							break;
						}
						NSString* gameID = [operationResponse objectForKey:[NSValue value:&ParameterCode::ROOM_NAME withObjCType:@encode(nByte)]];
						if(gameID) // is only sent by the server's response, if it has not been sent with the client's request before!
						{
							[mRoomName release];
							mRoomName = [gameID retain];
						}
						[mGameserver release];
						mGameserver = [[operationResponse objectForKey:[NSValue value:&ParameterCode::ADDRESS withObjCType:@encode(nByte)]] retain];
						mState = PeerStates::DisconnectingFromMasterserver;
						[super disconnect];
						mLastJoinType = JoinType::CREATE_ROOM;
					}
						break;
					case OperationCode::JOIN_ROOM:
						if(operationResponse.ReturnCode)
						{
							PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"opJoinRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.ReturnCode, operationResponse.DebugMessage.UTF32String);
							[mListener joinRoomReturn:0 :nil :nil :operationResponse.ReturnCode :operationResponse.DebugMessage];
							break;
						}
						[mGameserver release];
						mGameserver = [[operationResponse objectForKey:[NSValue value:&ParameterCode::ADDRESS withObjCType:@encode(nByte)]] retain];
						mState = PeerStates::DisconnectingFromMasterserver;
						[super disconnect];
						mLastJoinType = JoinType::JOIN_ROOM;
						break;
					default:
						break;
				}
			}
			break;
		case OperationCode::JOIN_RANDOM_ROOM:
			if(operationResponse.ReturnCode)
			{
				PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"opJoinRandomRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.ReturnCode, operationResponse.DebugMessage.UTF32String);
				[mListener joinRandomRoomReturn:0 :nil :nil :operationResponse.ReturnCode :operationResponse.DebugMessage];
				break;
			}
			// store the ID of the random game, joined on the masterserver, so that we know, which game to join on the gameserver
			[mRoomName release];
			mRoomName = [[operationResponse objectForKey:[NSValue value:&ParameterCode::ROOM_NAME withObjCType:@encode(nByte)]] retain];
			[mGameserver release];
			mGameserver = [[operationResponse objectForKey:[NSValue value:&ParameterCode::ADDRESS withObjCType:@encode(nByte)]] retain];
			mState = PeerStates::DisconnectingFromMasterserver;
			[super disconnect];
			mLastJoinType = JoinType::JOIN_RANDOM_ROOM;
			break;
		case OperationCode::JOIN_LOBBY:
			if(mLastJoinType == JoinType::AUTO_JOIN_LOBBY)
				[self onConnectToMasterFinished:mState==PeerStates::AuthenticatedComingFromGameserver];
			else
				[mListener joinLobbyReturn];
			mState = PeerStates::JoinedLobby;
			break;
		case OperationCode::LEAVE_LOBBY:
			mState = PeerStates::Authenticated;
			[mListener leaveLobbyReturn];
			break;
		case OperationCode::LEAVE:
			[mCurrentlyJoinedRoom setLocalPlayer:-1 :[NSDictionary dictionaryWithObject:mCurrentlyJoinedRoom.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]]];
			[mCurrentlyJoinedRoom removeAllPlayers];
			mState = PeerStates::DisconnectingFromGameserver;
			[super disconnect];
			break;
		default:
			break;
	}
}

- (void) onStatusChanged:(int)statusCode
{
	switch(statusCode)
	{
		case 0:
			break;
		case SC_CONNECT:
		{
			if(mState == PeerStates::ConnectingToGameserver)
			{
				PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_INFO, @"connected to gameserver");
				mState = PeerStates::ConnectedToGameserver;
			}
			else
			{
				PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_INFO, @"connected to masterserver");
				mState = mState==PeerStates::Connecting?PeerStates::Connected:PeerStates::ConnectedComingFromGameserver;
			}
			if(SEND_AUTHENTICATE_ENCRYPTED)
				[super establishEncryption];
			else
				[super opAuthenticate:mAppID :mAppVersion :false];
		}
			break;
		case SC_DISCONNECT:
		{
			if(mState == PeerStates::DisconnectingFromMasterserver)
			{
				[super connect:mGameserver];
				mState = PeerStates::ConnectingToGameserver;
			}
			else if(mState == PeerStates::DisconnectingFromGameserver)
			{
				[super connect:(NSString*)mMasterserver];
				mState = PeerStates::ConnectingToMasterserver;
			}
			else
			{
				mState = PeerStates::PeerCreated;
				[mListener disconnectReturn];
			}
		}
			break;
		case SC_ENCRYPTION_ESTABLISHED:
			[super opAuthenticate:mAppID :mAppVersion :true];
			break;
		case SC_ENCRYPTION_FAILED_TO_ESTABLISH:
			[self handleConnectionFlowError:mState :statusCode :@"Encryption failed to establish"];
			break;
		case SC_EXCEPTION:
		case SC_EXCEPTION_ON_CONNECT:
		case SC_INTERNAL_RECEIVE_EXCEPTION:
		case SC_TIMEOUT_DISCONNECT:
		case SC_DISCONNECT_BY_SERVER:
		case SC_DISCONNECT_BY_SERVER_USER_LIMIT:
		case SC_DISCONNECT_BY_SERVER_LOGIC:
			[mListener connectionErrorReturn:statusCode];
			if(self.Peerstate != PS_DISCONNECTED && self.Peerstate != PS_DISCONNECTING)
				[self disconnect];
			break;
		case SC_SEND_ERROR:
			[mListener clientErrorReturn:statusCode];
			break;
		case SC_QUEUE_OUTGOING_RELIABLE_WARNING:
		case SC_QUEUE_OUTGOING_UNRELIABLE_WARNING:
		case SC_QUEUE_OUTGOING_ACKS_WARNING:
		case SC_QUEUE_SENT_WARNING:
			[mListener warningReturn:statusCode];
			break;
		case ErrorCode::OPERATION_INVALID:
		case ErrorCode::INTERNAL_SERVER_ERROR:
			[mListener serverErrorReturn:statusCode];
			break;
		default:
			PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_ERRORS, @"received unknown status-code from server");
			break;
	}
}

- (void) onEvent:(EventData*)eventData
{
	PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_INFO, [eventData toString]); // don't print out the payload here, as that can get too expensive for big events
	switch(eventData.Code)
	{
		case EventCode::ROOM_LIST:
		{
			[mRoomList release];
			NSDictionary* roomList = [eventData objectForKey:[NSValue value:&ParameterCode::ROOM_LIST withObjCType:@encode(nByte)]];
			NSString** keys = new NSString*[roomList.count];
			NSDictionary** vals = new NSDictionary*[roomList.count];
			[roomList getObjects:vals andKeys:keys];
			mRoomList = [[NSMutableDictionary alloc] initWithCapacity:roomList.count];
			for(int i=0; i<roomList.count; i++)
				[mRoomList setObject:[LoadBalancingRoom loadBalancingRoom:keys[i] :vals[i]] forKey:keys[i]];
			delete[] keys;
			delete[] vals;
		}
			break;
		case EventCode::ROOM_LIST_UPDATE:
		{
			NSDictionary* roomListUpdate = [eventData objectForKey:[NSValue value:&ParameterCode::ROOM_LIST withObjCType:@encode(nByte)]];
			EGArray* keys = [EGArray arrayWithArray:roomListUpdate.allKeys];
			for(int i=0; i<keys.count; i++)
			{
				NSDictionary* val = [roomListUpdate objectForKey:[keys objectAtIndex:i]];
				bool removed = false;
				[(NSValue*)[val objectForKey:[NSValue value:&Properties::Room::REMOVED withObjCType:@encode(nByte)]] getValue:&removed];
				if(removed) // remove room
					[mRoomList removeObjectForKey:[keys objectAtIndex:i]];
				else
				{
					LoadBalancingRoom* room = [mRoomList objectForKey:[keys objectAtIndex:i]];
					if(room) // update room (only entries, which have been changed, have been sent)
						[room cacheProperties:val];
					else // add room
						[mRoomList setObject:[LoadBalancingRoom loadBalancingRoom:[keys objectAtIndex:i] :val] forKey:[keys objectAtIndex:i]];
				}
			}
		}
			break;
		case EventCode::QUEUE_STATE:
			[[eventData objectForKey:[NSValue value:&ParameterCode::POSITION withObjCType:@encode(nByte)]] getValue:&mQueuePosition];
			if(!mQueuePosition)
			{
				enum PeerStates::PeerStates oldState = mState;
				mState = PeerStates::Authenticated;
				
				if(oldState == PeerStates::QueuedComingFromGameserver)
					[mListener leaveRoomReturn:0 :nil];
				else if(mAutoJoinLobby)
					[self opJoinLobby];
				else
					[mListener connectReturn:0 :nil];
			}
			break;
		case EventCode::APP_STATS:
			[[eventData objectForKey:[NSValue value:&ParameterCode::PEER_COUNT withObjCType:@encode(nByte)]] getValue:&mPeerCount];
			[[eventData objectForKey:[NSValue value:&ParameterCode::ROOM_COUNT withObjCType:@encode(nByte)]] getValue:&mRoomCount];
			[[eventData objectForKey:[NSValue value:&ParameterCode::MASTER_PEER_COUNT withObjCType:@encode(nByte)]] getValue:&mMasterPeerCount];
			break;
		case EventCode::JOIN:
		{
			int nr = 0;
			[(NSValue*)[eventData objectForKey:[NSValue value:&ParameterCode::PLAYERNR withObjCType:@encode(nByte)]] getValue:&nr];
			LoadBalancingPlayer* player = [mCurrentlyJoinedRoom createPlayer:nr :[eventData objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]]];
			[mCurrentlyJoinedRoom addPlayer:player];
			[mListener joinRoomEventAction:nr :[eventData objectForKey:[NSValue value:&ParameterCode::PLAYER_LIST withObjCType:@encode(nByte)]] :player];
		}
			break;
		case EventCode::LEAVE:
		{
			int nr = -1;
			[(NSValue*)[eventData objectForKey:[NSValue value:&ParameterCode::PLAYERNR withObjCType:@encode(nByte)]] getValue:&nr];
			if(![mCurrentlyJoinedRoom removePlayerWithNumber:nr])
				PhotonPeer_sendDebugOutput(self, DEBUG_LEVEL_WARNINGS, @"LoadBalancingPeer::eventAction() EventCode::LEAVE - player %d, who is leaving the room, has not been found in list of players, who are currently in the room", nr);
			[mListener leaveRoomEventAction:nr];
		}
			break;
		case EventCode::PROPERTIES_CHANGED:
		{
			int target = 0;
			[(NSValue*)[eventData objectForKey:[NSValue value:&ParameterCode::TARGET_PLAYERNR withObjCType:@encode(nByte)]] getValue:&target];
			NSMutableDictionary* roomProperties = nil;
			NSMutableDictionary* playerProperties = nil;
			if(target)
				playerProperties = [eventData objectForKey:[NSValue value:&ParameterCode::PROPERTIES withObjCType:@encode(nByte)]];
			else
				roomProperties = [eventData objectForKey:[NSValue value:&ParameterCode::PROPERTIES withObjCType:@encode(nByte)]];
			[self readoutProperties:&roomProperties :&playerProperties :false :target];
		}
			break;
		default: // custom events are forwarded to the custom room code
		{
			int nr = 0;
			[(NSValue*)[eventData objectForKey:[NSValue value:&ParameterCode::PLAYERNR withObjCType:@encode(nByte)]] getValue:&nr];
			// custom event data is inside an inner hash
			[mListener customEventAction:nr :eventData.Code :[eventData objectForKey:[NSValue value:&ParameterCode::DATA withObjCType:@encode(nByte)]]];
		}
			break;
	}
}

- (void) debugReturn:(PhotonPeer_DebugLevel)debugLevel :(NSString*)string
{
	[mListener debugReturn:debugLevel :string];
}

@end



@implementation LoadBalancingClient (Internal)

- (bool) opSetPropertiesOfPlayer:(int)playerNr :(NSDictionary*)properties
{
	if(!self.IsInGameRoom)
		return false;
	return [super opSetPropertiesOfPlayer:(int)playerNr :(NSDictionary*)properties];
}

- (bool) opSetPropertiesOfRoom:(NSDictionary*)properties
{
	if(!self.IsInGameRoom)
		return false;
	return [super opSetPropertiesOfRoom:(NSDictionary*)properties];
}

@end



@implementation LoadBalancingClient (Private)

- (bool) IsOnGameServer
{
	return mState >= PeerStates::ConnectingToGameserver && mState < PeerStates::ConnectingToMasterserver;
}

- (void) readoutProperties:(NSMutableDictionary**)pRoomProperties :(NSMutableDictionary**)pPlayerProperties :(bool)multiplePlayers :(int)targetPlayerNr
{
	if(pRoomProperties)
	{
		if(*pRoomProperties)
			[mCurrentlyJoinedRoom cacheProperties:*pRoomProperties];
		*pRoomProperties = [LoadBalancingPeer stripKeysWithNullValues:[LoadBalancingPeer stripToCustomProperties:*pRoomProperties]];
	}
	if(pPlayerProperties)
	{
		if(*pPlayerProperties)
		{
			EGArray* players = nil;
			EGArray* playerNrs = nil;
			if(multiplePlayers && (*pPlayerProperties).count)
			{
				players = [EGArray arrayWithArray:[*pPlayerProperties allValues]];
				playerNrs = [EGArray arrayWithArray:[*pPlayerProperties allKeys]];
			}
			for(int i=0; i<(multiplePlayers?(*pPlayerProperties).count:1); i++)
			{
				NSDictionary* props = multiplePlayers?[players objectAtIndex:i]:*pPlayerProperties;
				if([props objectForKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]])
				{
					for(int j=0; j<mCurrentlyJoinedRoom.Players.count; j++)
					{
						int tmp;
						if(multiplePlayers)
							[[playerNrs objectAtIndex:i] getValue:&tmp];
						else
							tmp = targetPlayerNr;
						if(((LoadBalancingPlayer*)[mCurrentlyJoinedRoom.Players objectAtIndex:j]).Number == tmp)
						{
							[(LoadBalancingPlayer*)[mCurrentlyJoinedRoom.Players objectAtIndex:j] cacheProperties:props];
							if(mCurrentlyJoinedRoom.LocalPlayer.Number == ((LoadBalancingPlayer*)[mCurrentlyJoinedRoom.Players objectAtIndex:j]).Number)
								mCurrentlyJoinedRoom.LocalPlayer = [mCurrentlyJoinedRoom.Players objectAtIndex:j];
						}
					}
				}
			}
		}
		if(multiplePlayers)
		{
			for(NSUInteger i=0; i<(*pPlayerProperties).count; i++)
			{
				id key = [[*pPlayerProperties allKeys] objectAtIndex:i];
				[*pPlayerProperties setObject:[LoadBalancingPeer stripKeysWithNullValues:[LoadBalancingPeer stripToCustomProperties:[*pPlayerProperties objectForKey:key]]] forKey:key];
			}
		}
		else
			*pPlayerProperties = [LoadBalancingPeer stripKeysWithNullValues:[LoadBalancingPeer stripToCustomProperties:*pPlayerProperties]];
	}
}

- (void) handleConnectionFlowError:(PeerStates::PeerStates)oldState :(int)errorCode :(const NSString* const)errorString
{
	if(oldState == PeerStates::ConnectedToGameserver || oldState == PeerStates::AuthenticatedOnGameServer || oldState == PeerStates::Joining)
	{
		mCachedErrorCodeFromGameServer = errorCode;
		[mCachedErrorStringFromGameServer release];
		mCachedErrorStringFromGameServer = [errorString retain];
		mState = PeerStates::DisconnectingFromGameserver;
		[super disconnect];
		// response to app has to wait until back on master
	}
	else
	{
		mState = PeerStates::Disconnecting;
		[super disconnect];
		[mListener connectReturn:errorCode :errorString];
	}
}

- (void) onConnectToMasterFinished:(bool)comingFromGameserver
{
	if(comingFromGameserver)
	{
		if(mCachedErrorCodeFromGameServer)
		{
			switch(mLastJoinType)
			{
				case JoinType::CREATE_ROOM:
					[mListener createRoomReturn:0 :nil :nil :mCachedErrorCodeFromGameServer :mCachedErrorStringFromGameServer];
					break;
				case JoinType::JOIN_ROOM:
					[mListener joinRoomReturn:0 :nil :nil :mCachedErrorCodeFromGameServer :mCachedErrorStringFromGameServer];
					break;
				case JoinType::JOIN_RANDOM_ROOM:
					[mListener joinRandomRoomReturn:0 :nil :nil :mCachedErrorCodeFromGameServer :mCachedErrorStringFromGameServer];
					break;
				default:
					break;
			}
			mCachedErrorCodeFromGameServer = LoadBalancing::ErrorCode::OK;
			[mCachedErrorStringFromGameServer release];
			mCachedErrorStringFromGameServer = nil;
		}
		else
			[mListener leaveRoomReturn:0 :nil];
	}
	else
		[mListener connectReturn:0 :nil];
}

@end