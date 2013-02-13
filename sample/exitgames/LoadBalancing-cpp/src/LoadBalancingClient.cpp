/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2012 by Exit Games. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancingClient.h"
#include "Internal/Enums/Properties/Room.h"
#include "Internal/Enums/Properties/Player.h"
#include "Internal/Enums/ParameterCode.h"
#include "Internal/Enums/OperationCode.h"
#include "Internal/Enums/JoinType.h"
#include "Internal/Enums/EventCode.h"

/** @file LoadBalancingClient.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;
		using namespace Photon;

		// default master server address and port
		const char* const MASTERSERVER = "app.exitgamescloud.com";
		const char* const UDP_PORT = ":5055";
		const char* const TCP_PORT = ":4530";
		
		const bool SEND_AUTHENTICATE_ENCRYPTED = true;

		PeerStates::PeerStates Client::getState(void) const
		{
			return mState;
		}

		const JString& Client::getMasterserverAddress(void) const
		{
			return mMasterserver;
		}

		int Client::getCountPlayersIngame(void) const
		{
			return mPeerCount;
		}

		int Client::getCountGamesRunning(void) const
		{
			return mRoomCount;
		}

		int Client::getCountPlayersOnline(void) const
		{
			return mPeerCount + mMasterPeerCount;
		}

		int Client::getQueuePosition(void) const
		{
			return mQueuePosition;
		}

		MutableRoom& Client::getCurrentlyJoinedRoom(void)
		{
			return mCurrentlyJoinedRoom;
		}

		const JVector<Room>& Client::getRoomList(void) const
		{
			return mRoomList;
		}
		
		const JVector<JString>& Client::getRoomNameList(void) const
		{
			return mRoomNameList;
		}

		bool Client::getIsInRoom(void) const
		{
			return getIsInGameRoom() || getIsInLobby();
		}

		bool Client::getIsInGameRoom(void) const
		{
			return mState == PeerStates::Joined;
		}

		bool Client::getIsInLobby(void) const
		{
			return mState == PeerStates::JoinedLobby;
		}

		bool Client::getAutoJoinLobby(void) const
		{
			return mAutoJoinLobby;
		}

		void Client::setAutoJoinLobby(bool onConnect)
		{
			mAutoJoinLobby = onConnect;
		}

		Client::Client(LoadBalancing::Listener& listener, const JString& applicationID, const JString& appVersion, const JString& username, bool useTcp)
#if defined _EG_MS_COMPILER
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif
			: Peer(this, useTcp)
			, mListener(listener)
			, mAppVersion(appVersion)
			, mAppID(applicationID)
			, mPeerCount(0)
			, mRoomCount(0)
			, mMasterPeerCount(0)
			, mLastJoinType(0)
			, mQueuePosition(0)
			, mCurrentlyJoinedRoom("", Hashtable(), this, JVector<JString>())
			, mSenttime(0)
			, mCachedErrorCodeFromGameServer(ErrorCode::OK)
			, mUseTcp(useTcp)
			, mAutoJoinLobby(true)
			, mState(PeerStates::Uninitialized)
#ifdef _EG_MS_COMPILER
#	pragma warning(pop)
#endif
		{
			mCurrentlyJoinedRoom.getLocalPlayer().setName(username);
		}

		Client::~Client(void)
		{
		}

		bool Client::connect(void)
		{
			return connect(JString(MASTERSERVER) + (mUseTcp?TCP_PORT:UDP_PORT));
		}

		bool Client::connect(const JString& masterServerAddress)
		{
			mState = PeerStates::Connecting;
			return super::connect(mMasterserver=masterServerAddress);
		}

		void Client::disconnect(void)
		{
			mState = PeerStates::Disconnecting;
			super::disconnect();
		}

		bool Client::opJoinLobby(void)
		{
			if(getIsInRoom())
			{
				PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"already in a room");
				return false;
			}

			mLastJoinType = JoinType::EXPLICIT_JOIN_LOBBY;
			return super::opCustom(OperationRequest(OperationCode::JOIN_LOBBY), true);
		}

		bool Client::opLeaveLobby(void)
		{
			if(!getIsInLobby())
			{
				PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"lobby isn't currently joined");
				return false;
			}

			return super::opCustom(OperationRequest(OperationCode::LEAVE_LOBBY), true);
		}

		bool Client::opCreateRoom(const JString& gameID, bool isVisible, bool isOpen, nByte maxPlayers, const Hashtable& customRoomProperties, const Hashtable& customLocalPlayerProperties, const JVector<JString>& propsListedInLobby)
		{
			if(getIsInGameRoom())
			{
				PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"already in a gameroom");
				return false;
			}

			mRoomName = gameID;

			OperationRequestParameters op(opCreateRoomImplementation(gameID, isVisible, isOpen, maxPlayers, getIsOnGameServer()?customRoomProperties:Hashtable(), getIsOnGameServer()?customLocalPlayerProperties:Hashtable(), getIsOnGameServer()?propsListedInLobby:JVector<JString>()));
			if(mCurrentlyJoinedRoom.getLocalPlayer().getName().length())
			{
				if((ValueObject<Hashtable>*)op.getValue(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES)))
					((ValueObject<Hashtable>*)op.getValue(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES)))->getDataAddress()->put(static_cast<nByte>(Properties::Player::PLAYERNAME), mCurrentlyJoinedRoom.getLocalPlayer().getName());
				else
				{
					Hashtable playerProp;
					playerProp.put(static_cast<nByte>(Properties::Player::PLAYERNAME), mCurrentlyJoinedRoom.getLocalPlayer().getName());
					op.put(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES), ValueObject<Hashtable>(playerProp));
				}
			}

			if(!opCustom(OperationRequest(OperationCode::CREATE_ROOM, op), true))
				return false;

			Hashtable roomProps(stripToCustomProperties(customRoomProperties));
			roomProps.put(static_cast<nByte>(Properties::Room::IS_OPEN), isOpen);
			roomProps.put(static_cast<nByte>(Properties::Room::IS_VISIBLE), isVisible);
			roomProps.put(static_cast<nByte>(Properties::Room::MAX_PLAYERS), maxPlayers);
			JString* propsListedInLobbyArr = new JString[propsListedInLobby.getSize()];
			for(unsigned int i=0; i<propsListedInLobby.getSize(); ++i)
				propsListedInLobbyArr[i] = propsListedInLobby[i];
			roomProps.put(static_cast<nByte>(Properties::Room::PROPS_LISTED_IN_LOBBY), propsListedInLobbyArr, propsListedInLobby.getSize());
			delete[] propsListedInLobbyArr;

			MutablePlayer localPlayer = mCurrentlyJoinedRoom.getLocalPlayer();
			mCurrentlyJoinedRoom = MutableRoom(gameID, roomProps, this, propsListedInLobby);
			localPlayer.cacheProperties(customLocalPlayerProperties);
			mCurrentlyJoinedRoom.setLocalPlayer(localPlayer);
			return true;
		}

		bool Client::opJoinRoom(const JString& gameID, const Hashtable& customLocalPlayerProperties)
		{
			if(getIsInGameRoom())
			{
				PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"already in a gameroom");
				return false;
			}

			mRoomName = gameID;
			OperationRequestParameters op = opJoinRoomImplementation(gameID, getIsOnGameServer()?customLocalPlayerProperties:Hashtable());
			if(mCurrentlyJoinedRoom.getLocalPlayer().getName().length())
			{
				if((ValueObject<Hashtable>*)op.getValue(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES)))
					((ValueObject<Hashtable>*)op.getValue(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES)))->getDataAddress()->put(static_cast<nByte>(Properties::Player::PLAYERNAME), mCurrentlyJoinedRoom.getLocalPlayer().getName());
				else
				{
					Hashtable playerProp;
					playerProp.put(static_cast<nByte>(Properties::Player::PLAYERNAME), mCurrentlyJoinedRoom.getLocalPlayer().getName());
					op.put(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES), ValueObject<Hashtable>(playerProp));
				}
			}
			if(!gameID.length() || !super::opCustom(OperationRequest(OperationCode::JOIN_ROOM, op), true))
				return false;
			
			MutablePlayer localPlayer = mCurrentlyJoinedRoom.getLocalPlayer();
			mCurrentlyJoinedRoom = MutableRoom(gameID, Hashtable(), this);
			localPlayer.cacheProperties(customLocalPlayerProperties);
			mCurrentlyJoinedRoom.setLocalPlayer(localPlayer);
			return true;
		}

		bool Client::opJoinRandomRoom(const Hashtable& customRoomProperties, nByte maxPlayers, const Hashtable& customLocalPlayerProperties)
		{
			if(getIsInGameRoom())
			{
				PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"already in a gameroom");
				return false;
			}
			
			if(!super::opJoinRandomRoom(customRoomProperties, maxPlayers))
				return false;

			MutablePlayer localPlayer = mCurrentlyJoinedRoom.getLocalPlayer();
			mCurrentlyJoinedRoom = MutableRoom("", stripToCustomProperties(customRoomProperties), this);
			localPlayer.mCustomProperties.removeAllElements();
			localPlayer.cacheProperties(customLocalPlayerProperties);
			mCurrentlyJoinedRoom.setLocalPlayer(localPlayer);
			return true;
		}

		bool Client::opLeaveRoom(void)
		{
			if(!getIsInGameRoom())
			{
				PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"no gameroom is currently joined");
				return false;
			}

			if(!super::opLeaveRoom())
				return false;

			mState = PeerStates::Leaving;
			return true;
		}

		// protocol implementations
		void Client::onOperationResponse(const OperationResponse& operationResponse)
		{
			PhotonPeer_sendDebugOutput(this, operationResponse.getReturnCode()?DEBUG_LEVEL_ERRORS:DEBUG_LEVEL_INFO, operationResponse.toString(true, true));
			switch(operationResponse.getOperationCode())
			{
			case OperationCode::AUTHENTICATE:
				{
					PeerStates::PeerStates oldState = mState;
					if(operationResponse.getReturnCode())
					{
						PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"authentication failed with errorcode %d: %ls", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
						handleConnectionFlowError(oldState, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
						break;
					}
					else
					{
						if(mState == PeerStates::Connected || mState == PeerStates::ConnectedComingFromGameserver)
						{
							ValueObject<int> queueParam = operationResponse.getParameterForCode(ParameterCode::POSITION);
							mQueuePosition = queueParam.getDataCopy();
							if(mQueuePosition) // should only happen, if just out of nowhere the amount of players going online at the same time is increasing faster, than automatically started additional gameservers could have been booten up
							{
								mState = PeerStates::Connected?PeerStates::Queued:PeerStates::QueuedComingFromGameserver;
								mListener.gotQueuedReturn();
								break;
							}
							mState = PeerStates::Connected?PeerStates::Authenticated:PeerStates::AuthenticatedComingFromGameserver;
							if(mAutoJoinLobby)
							{
								opJoinLobby();
								mLastJoinType = JoinType::AUTO_JOIN_LOBBY;
							}
							else
								onConnectToMasterFinished(oldState==PeerStates::ConnectedComingFromGameserver);
						}
						else if(mState == PeerStates::ConnectedToGameserver)
						{
							mState = PeerStates::Joining;
							if(mLastJoinType == JoinType::CREATE_ROOM)
								opCreateRoom(mRoomName, mCurrentlyJoinedRoom.getIsVisible(), mCurrentlyJoinedRoom.getIsOpen(), mCurrentlyJoinedRoom.getMaxPlayers(), mCurrentlyJoinedRoom.getCustomProperties(), mCurrentlyJoinedRoom.getLocalPlayer().getCustomProperties(), mCurrentlyJoinedRoom.getPropsListedInLobby());
							else
								opJoinRoom(mRoomName, mCurrentlyJoinedRoom.getLocalPlayer().getCustomProperties());
						}
					}
				}

				break;
			case OperationCode::CREATE_ROOM:
			case OperationCode::JOIN_ROOM:
				{
					if(getIsOnGameServer())
					{
						if(operationResponse.getReturnCode())
						{
							PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"%ls failed with errorcode %d: %ls. Client is therefore returning to masterserver!", operationResponse.getOperationCode()==OperationCode::CREATE_ROOM?L"opCreateRoom":L"opJoinRoom", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
							handleConnectionFlowError(mState, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						}

						mState = PeerStates::Joined;
						int nr = ValueObject<int>(operationResponse.getParameterForCode(ParameterCode::PLAYERNR)).getDataCopy();
						Hashtable properties;
						properties.put(static_cast<nByte>(Properties::Player::PLAYERNAME), mCurrentlyJoinedRoom.getLocalPlayer().getName());
						mCurrentlyJoinedRoom.setLocalPlayer(nr, properties);
						mCurrentlyJoinedRoom.setPlayers(JVector<Player>());

						Hashtable roomProperties(ValueObject<Hashtable>(operationResponse.getParameterForCode(ParameterCode::ROOM_PROPERTIES)).getDataCopy());
						Hashtable playerProperties(ValueObject<Hashtable>(operationResponse.getParameterForCode(ParameterCode::PLAYER_PROPERTIES)).getDataCopy());
						JVector<Object> numbers = playerProperties.getKeys();
						for(unsigned int i=0 ; i<numbers.getSize() ; ++i)
							mCurrentlyJoinedRoom.addPlayer((((KeyObject<int>)numbers[i]).getDataCopy()), ValueObject<Hashtable>(playerProperties.getValue(numbers[i])).getDataCopy());
						readoutProperties(roomProperties, playerProperties, true, 0);

						switch(mLastJoinType)
						{
						case JoinType::CREATE_ROOM:
							mListener.createRoomReturn(nr, roomProperties, playerProperties, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						case JoinType::JOIN_ROOM:
							mListener.joinRoomReturn(nr, roomProperties, playerProperties, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						case JoinType::JOIN_RANDOM_ROOM:
							mListener.joinRandomRoomReturn(nr, roomProperties, playerProperties, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						default:
							break;
						}
						break;
					}
					else
					{
						switch(operationResponse.getOperationCode())
						{
						case OperationCode::CREATE_ROOM:
							{
								if(operationResponse.getReturnCode())
								{
									PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"opCreateRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
									mListener.createRoomReturn(0, Hashtable(), Hashtable(), operationResponse.getReturnCode(), operationResponse.getDebugMessage());
									break;
								}
								JString gameID = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ROOM_NAME)).getDataCopy();
								if(gameID.length()) // is only sent by the server's response, if it has not been sent with the client's request before!
									mRoomName = gameID;

								mGameserver = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataCopy();
								mState = PeerStates::DisconnectingFromMasterserver;
								super::disconnect();
								mLastJoinType = JoinType::CREATE_ROOM;
							}
							break;
						case OperationCode::JOIN_ROOM:
							if(operationResponse.getReturnCode())
							{
								PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"opJoinRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
								mListener.joinRoomReturn(0, Hashtable(), Hashtable(), operationResponse.getReturnCode(), operationResponse.getDebugMessage());
								break;
							}

							mGameserver = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataCopy();
							mState = PeerStates::DisconnectingFromMasterserver;
							super::disconnect();
							mLastJoinType = JoinType::JOIN_ROOM;
							break;
						default:
							break;
						}
					}
				}
				break;
			case OperationCode::JOIN_RANDOM_ROOM:
				{
					if(operationResponse.getReturnCode())
					{
						PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"opJoinRandomRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
						mListener.joinRandomRoomReturn(0, Hashtable(), Hashtable(), operationResponse.getReturnCode(), operationResponse.getDebugMessage());
						break;
					}

					// store the ID of the random game, joined on the masterserver, so that we know, which game to join on the gameserver
					mRoomName = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ROOM_NAME)).getDataCopy();
					mGameserver = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataCopy();
					mState = PeerStates::DisconnectingFromMasterserver;
					super::disconnect();
					mLastJoinType = JoinType::JOIN_RANDOM_ROOM;
				}
				break;
			case OperationCode::JOIN_LOBBY:
				if(mLastJoinType == JoinType::AUTO_JOIN_LOBBY)
					onConnectToMasterFinished(mState==PeerStates::AuthenticatedComingFromGameserver);
				else
					mListener.joinLobbyReturn();
				mState = PeerStates::JoinedLobby;
				break;
			case OperationCode::LEAVE_LOBBY:
				{
					mState = PeerStates::Authenticated;
					mListener.leaveLobbyReturn();
				}
				break;
			case OperationCode::LEAVE:
				{
					Hashtable op;
					op.put(static_cast<nByte>(Properties::Player::PLAYERNAME), mCurrentlyJoinedRoom.getLocalPlayer().getName());
					mCurrentlyJoinedRoom.setLocalPlayer(-1, op);
					mCurrentlyJoinedRoom.removeAllPlayers();
					mState = PeerStates::DisconnectingFromGameserver;
					super::disconnect();
				}
				break;
			default:
				break;
			}
		}

		void Client::onStatusChanged(int statusCode)
		{
			switch(statusCode)
			{
			case 0:
				break;
			case SC_CONNECT:
				{
					if(mState == PeerStates::ConnectingToGameserver)
					{
						PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_INFO, L"connected to gameserver");
						mState = PeerStates::ConnectedToGameserver;
					}
					else
					{
						PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_INFO, L"connected to masterserver");
						mState = mState == PeerStates::Connecting ? PeerStates::Connected : PeerStates::ConnectedComingFromGameserver;
					}
					if(SEND_AUTHENTICATE_ENCRYPTED)
						super::establishEncryption();
					else
						super::opAuthenticate(mAppID, mAppVersion, false);
				}
				break;
			case SC_DISCONNECT:
				{
					if(mState == PeerStates::DisconnectingFromMasterserver)
					{
						super::connect(mGameserver);
						mState = PeerStates::ConnectingToGameserver;
					}
					else if(mState == PeerStates::DisconnectingFromGameserver)
					{
						super::connect(mMasterserver);
						mState = PeerStates::ConnectingToMasterserver;
					}
					else
					{
						mState = PeerStates::PeerCreated;
						mListener.disconnectReturn();
					}
				}
				break;
			case SC_ENCRYPTION_ESTABLISHED:
				super::opAuthenticate(mAppID, mAppVersion, true);
				break;
			case SC_ENCRYPTION_FAILED_TO_ESTABLISH:
				handleConnectionFlowError(mState, statusCode, "Encryption failed to establish");
				break;
			case SC_EXCEPTION:
			case SC_EXCEPTION_ON_CONNECT:
			case SC_INTERNAL_RECEIVE_EXCEPTION:
			case SC_TIMEOUT_DISCONNECT:
			case SC_DISCONNECT_BY_SERVER:
			case SC_DISCONNECT_BY_SERVER_USER_LIMIT:
			case SC_DISCONNECT_BY_SERVER_LOGIC:
				mListener.connectionErrorReturn(statusCode);
				if(this->getPeerState() != PS_DISCONNECTED && this->getPeerState() != PS_DISCONNECTING)
					disconnect();
				break;
			case SC_SEND_ERROR:
				mListener.clientErrorReturn(statusCode);
				break;
			case SC_QUEUE_OUTGOING_RELIABLE_WARNING:
			case SC_QUEUE_OUTGOING_UNRELIABLE_WARNING:
			case SC_QUEUE_OUTGOING_ACKS_WARNING:
			case SC_QUEUE_SENT_WARNING:
				mListener.warningReturn(statusCode);
				break;
			case ErrorCode::OPERATION_INVALID:
			case ErrorCode::INTERNAL_SERVER_ERROR:
				mListener.serverErrorReturn(statusCode);
				break;
			default:
				PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_ERRORS, L"received unknown status-code from server");
				break;
			}
		}

		void Client::onEvent(const EventData& eventData)
		{
			PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_INFO, eventData.toString()); // don't print out the payload here, as that can get too expensive for big events
			switch(eventData.getCode())
			{
			case EventCode::ROOM_LIST:
				{
					mRoomList.removeAllElements();
					mRoomNameList.removeAllElements();

					Hashtable roomList = ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::ROOM_LIST)).getDataCopy();
					JVector<Object> keys = roomList.getKeys();
					JString roomNameValue;
					for(unsigned int i=0 ; i<keys.getSize(); ++i)
					{
						roomNameValue = KeyObject<JString>(keys[i]).getDataCopy();
						mRoomList.addElement(Room(roomNameValue, ValueObject<Hashtable>(roomList.getValue(keys[i])).getDataCopy()));
						mRoomNameList.addElement(roomNameValue);
					}
				}
				break;
			case EventCode::ROOM_LIST_UPDATE:
				{
					Hashtable roomListUpdate(ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::ROOM_LIST)).getDataCopy());
					JVector<Object> keys = roomListUpdate.getKeys();
					for(unsigned int i=0; i<keys.getSize(); i++)
					{
						Hashtable val(ValueObject<Hashtable>(roomListUpdate.getValue(keys[i])).getDataCopy());
						bool removed = ValueObject<bool>(val.getValue(static_cast<nByte>(Properties::Room::REMOVED))).getDataCopy();
						int index = mRoomNameList.getIndexOf(KeyObject<JString>(keys[i]).getDataCopy());

						if(!removed)
						{
							if(index == -1) //add room
							{
								JString roomNameValue = KeyObject<JString>(keys[i]).getDataCopy();
								mRoomList.addElement(Room(roomNameValue, val));
								mRoomNameList.addElement(roomNameValue);
							}
							else // update room (only entries, which have been changed, have been sent)
								mRoomList.getElementAt(index).cacheProperties(val);
						}
						else if(index > -1) // remove room
						{
							mRoomList.removeElementAt(index);
							mRoomNameList.removeElementAt(index);
						}
					}
				}
				break;
			case EventCode::QUEUE_STATE:
				{
					mQueuePosition = ValueObject<int>(eventData.getParameterForCode(ParameterCode::POSITION)).getDataCopy();
					if(!mQueuePosition)
					{
						enum PeerStates::PeerStates oldState = mState;
						mState = PeerStates::Authenticated;

						if(oldState == PeerStates::QueuedComingFromGameserver)
							mListener.leaveRoomReturn(0, "");
						else if(mAutoJoinLobby)
							opJoinLobby();
						else
							mListener.connectReturn(0, "");
					}
				}
				break;
			case EventCode::APP_STATS:
				{
					mPeerCount = ValueObject<int>(eventData.getParameterForCode(ParameterCode::PEER_COUNT)).getDataCopy();
					mRoomCount = ValueObject<int>(eventData.getParameterForCode(ParameterCode::ROOM_COUNT)).getDataCopy();
					mMasterPeerCount = ValueObject<int>(eventData.getParameterForCode(ParameterCode::MASTER_PEER_COUNT)).getDataCopy();
				}
				break;
			case EventCode::JOIN:
				{
					int nr = ValueObject<int>(eventData.getParameterForCode(ParameterCode::PLAYERNR)).getDataCopy();
					Hashtable playerProperties(ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::PLAYER_PROPERTIES)).getDataCopy());
					mCurrentlyJoinedRoom.addPlayer(nr, playerProperties);
					int* players = ValueObject<int*>(eventData.getParameterForCode(ParameterCode::PLAYER_LIST)).getDataCopy();
					JVector<int> playerArr;
					int* playersPtr = players;
					for(int i=0 ; i<nr ; ++i, playersPtr++)
						playerArr.addElement(*playersPtr);
					mListener.joinRoomEventAction(nr, playerArr, *mCurrentlyJoinedRoom.getPlayerForNumber(nr));
					FREE(players);
				}
				break;
			case EventCode::LEAVE:
				{
					ValueObject<int> nr = eventData.getParameterForCode(ParameterCode::PLAYERNR);
					if(!mCurrentlyJoinedRoom.removePlayer(nr.getDataCopy()))
						PhotonPeer_sendDebugOutput(this, DEBUG_LEVEL_WARNINGS, L"LoadBalancingsuper::eventAction() EventCode::LEAVE - player %d who is leaving the room, has not been found in list of players, who are currently in the room", nr.getDataCopy());
					mListener.leaveRoomEventAction(nr.getDataCopy());
				}
				break;
			case EventCode::PROPERTIES_CHANGED:
				{
					ValueObject<int> target = eventData.getParameterForCode(ParameterCode::TARGET_PLAYERNR);
					Hashtable roomProperties;
					Hashtable playerProperties;
					if(target.getDataCopy())
						playerProperties = ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::PROPERTIES)).getDataCopy();
					else
						roomProperties = ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::PROPERTIES)).getDataCopy();
					readoutProperties(roomProperties, playerProperties, false, target.getDataCopy());
				}
				break;
			default: // custom events are forwarded to the custom room code
				{
					ValueObject<int> nr = eventData.getParameterForCode(ParameterCode::PLAYERNR);
					// custom event data is inside an inner hash
					mListener.customEventAction(nr.getDataCopy(), eventData.getCode(), ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::DATA)).getDataCopy());
				}
				break;
			}
		}

		void Client::debugReturn(PhotonPeer_DebugLevel debugLevel, const JString& string)
		{
			mListener.debugReturn(debugLevel, string);
		}

		bool Client::getIsOnGameServer(void) const
		{
			return mState >= PeerStates::ConnectingToGameserver && mState < PeerStates::ConnectingToMasterserver;
		}

		void Client::readoutProperties(Hashtable& roomProperties, Hashtable& playerProperties, bool multiplePlayers, int targetPlayerNr)
		{
			if(roomProperties.getSize())
			{
				mCurrentlyJoinedRoom.cacheProperties(roomProperties);
				roomProperties = stripKeysWithNullValues(stripToCustomProperties(roomProperties));
			}

			if(playerProperties.getSize())
			{
				JVector<Object> players;
				JVector<Object> playersNrs;
				if(multiplePlayers)
				{
					playersNrs = playerProperties.getKeys();
					unsigned int i=0;
					for(i=0; i<playersNrs.getSize(); ++i)
						players.addElement(playerProperties[i]);
					for(i=0; i<(multiplePlayers?playerProperties.getSize():1); ++i)
					{
						Hashtable props = multiplePlayers?ValueObject<Hashtable>(players[i]).getDataCopy():playerProperties;
						if(props.contains(static_cast<nByte>(Properties::Player::PLAYERNAME)))
						{
							for(unsigned int j=0; j<mCurrentlyJoinedRoom.getPlayers().getSize(); j++)
							{
								if(mCurrentlyJoinedRoom.getPlayers()[j].getNumber() == (multiplePlayers?ValueObject<int>(playersNrs[i]).getDataCopy():targetPlayerNr))
								{
									mCurrentlyJoinedRoom.setPlayerForNumber(mCurrentlyJoinedRoom.getPlayers()[j].getNumber(), props);
									if(mCurrentlyJoinedRoom.getLocalPlayer().getNumber() == mCurrentlyJoinedRoom.getPlayers()[j].getNumber())
									{
										mCurrentlyJoinedRoom.setLocalPlayer(mCurrentlyJoinedRoom.getPlayers()[j].getNumber(), mCurrentlyJoinedRoom.getPlayers()[j].getCustomProperties());
										mCurrentlyJoinedRoom.getLocalPlayer().setName(mCurrentlyJoinedRoom.getPlayers()[j].getName());
									}
								}
							}
						}
					}
				}
			}

			if(multiplePlayers)
				for(unsigned int i=0; i<playerProperties.getSize(); i++)
					playerProperties[i] = ValueObject<Hashtable>(stripKeysWithNullValues(stripToCustomProperties(ValueObject<Hashtable>(playerProperties[i]).getDataCopy())));
			else
				playerProperties = stripKeysWithNullValues(stripToCustomProperties(playerProperties));
		}

		void Client::handleConnectionFlowError(PeerStates::PeerStates oldState, int errorCode, const JString& errorString)
		{
			if(oldState == PeerStates::ConnectedToGameserver || oldState == PeerStates::AuthenticatedOnGameServer || oldState == PeerStates::Joining)
			{
				mCachedErrorCodeFromGameServer = errorCode;
				mCachedErrorStringFromGameServer = errorString;
				mState = PeerStates::DisconnectingFromGameserver;
				super::disconnect();
				// response to app has to wait until back on master
			}
			else
			{
				mState = PeerStates::Disconnecting;
				super::disconnect();
				mListener.connectReturn(errorCode, errorString);
			}
		}

		bool Client::opAuthenticate(const JString& appID, const JString& appVersion, bool encrypted)
		{
			return super::opAuthenticate(appID, appVersion, encrypted);
		}

		bool Client::opSetPropertiesOfPlayer(int playerNr, const Hashtable& properties)
		{
			if(!getIsInGameRoom())
				return false;
			return super::opSetPropertiesOfPlayer(playerNr, properties);
		}

		bool Client::opSetPropertiesOfRoom(const Hashtable& properties)
		{
			if(!getIsInGameRoom())
				return false;
			return super::opSetPropertiesOfRoom(properties);
		}

		void Client::onConnectToMasterFinished(bool comingFromGameserver)
		{
			if(comingFromGameserver)
			{
				if(mCachedErrorCodeFromGameServer)
				{
					switch(mLastJoinType)
					{
						case JoinType::CREATE_ROOM:
							mListener.createRoomReturn(0, Hashtable(), Hashtable(), mCachedErrorCodeFromGameServer, mCachedErrorStringFromGameServer);
							break;
						case JoinType::JOIN_ROOM:
							mListener.joinRoomReturn(0, Hashtable(), Hashtable(), mCachedErrorCodeFromGameServer, mCachedErrorStringFromGameServer);
							break;
						case JoinType::JOIN_RANDOM_ROOM:
							mListener.joinRandomRoomReturn(0, Hashtable(), Hashtable(), mCachedErrorCodeFromGameServer, mCachedErrorStringFromGameServer);
							break;
						default:
							break;
					}
					mCachedErrorCodeFromGameServer = LoadBalancing::ErrorCode::OK;
					mCachedErrorStringFromGameServer = "";
				}
				else
					mListener.leaveRoomReturn(0, "");
			}
			else
				mListener.connectReturn(0, "");
		}
	}
}
