/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2012 by Exit Games. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancingPeer.h"
#include "Internal/Enums/Properties/Room.h"
#include "Internal/Enums/Properties/Player.h"
#include "Internal/Enums/ParameterCode.h"
#include "Internal/Enums/OperationCode.h"
#include "Internal/Enums/JoinType.h"
#include "Enums/ReceiverGroup.h"
#include "Enums/EventCache.h"
#include "Enums/DebugLevel.h"
#include "Enums/ParameterCode.h"

/** @file LoadBalancingPeer.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;
		using namespace Common::MemoryManagement;
		using namespace Photon;
		using namespace Lite;
		using namespace Lite::ParameterCode;
		using namespace Internal;

		Peer::Peer(Photon::PhotonListener& listener, bool useTcp)
			: PhotonPeer(listener, useTcp)
		{
		}

		Peer::~Peer(void)
		{
		}

		bool Peer::opJoinLobby(void)
		{
			return super::opCustom(OperationRequest(OperationCode::JOIN_LOBBY), true);
		}

		bool Peer::opLeaveLobby(void)
		{
			return super::opCustom(OperationRequest(OperationCode::LEAVE_LOBBY), true);
		}

		bool Peer::opCreateRoom(const JString& gameID, bool isVisible, bool isOpen, nByte maxPlayers, const Hashtable& customRoomProperties, const Hashtable& customLocalPlayerProperties, const JVector<JString>& propsListedInLobby)
		{
			return super::opCustom(OperationRequest(OperationCode::CREATE_ROOM, opCreateRoomImplementation(gameID, isVisible, isOpen, maxPlayers, customRoomProperties, customLocalPlayerProperties, propsListedInLobby)), true);
		}

		bool Peer::opJoinRoom(const JString& gameID, const Hashtable& customLocalPlayerProperties)
		{
			return gameID.length() ? super::opCustom(OperationRequest(OperationCode::JOIN_ROOM, opJoinRoomImplementation(gameID, customLocalPlayerProperties)), true) : false;
		}
		
		bool Peer::opJoinRandomRoom(const Hashtable& customRoomProperties, nByte maxPlayers)
		{
			Hashtable roomProps(stripToCustomProperties(customRoomProperties));
			if(maxPlayers)
				roomProps.put(static_cast<nByte>(Properties::Room::MAX_PLAYERS), maxPlayers);

			OperationRequest opRequest(OperationCode::JOIN_RANDOM_ROOM);
			
			if(roomProps.getSize())
			{
				OperationRequestParameters op;
				op.put((nByte)ParameterCode::ROOM_PROPERTIES, ValueObject<Hashtable>(roomProps));
				opRequest.setParameters(op);
			}
			
			return super::opCustom(opRequest, true);
		}

		bool Peer::opLeaveRoom(void)
		{
			return super::opCustom(OperationRequest(OperationCode::LEAVE), true);
		}

		bool Peer::opRaiseEvent(bool reliable, const Hashtable& parameters, nByte eventCode, nByte channelID, int* targetPlayers, short numTargetPlayers)
		{
			return opRaiseEventImplementation(reliable, parameters, eventCode, channelID, targetPlayers, numTargetPlayers, EventCache::DO_NOT_CACHE, ReceiverGroup::OTHERS);
		}

		bool Peer::opRaiseEvent(bool reliable, const Hashtable& parameters, nByte eventCode, nByte channelID, nByte eventCaching, nByte receiverGroup)
		{
			return opRaiseEventImplementation(reliable, parameters, eventCode, channelID, NULL, 0, eventCaching, receiverGroup);
		}

		bool Peer::opAuthenticate(const JString& appID, const JString& appVersion, bool encrypted)
		{
			OperationRequestParameters op;
			op.put((nByte)ParameterCode::APPLICATION_ID, ValueObject<JString>(appID));
			op.put((nByte)ParameterCode::APP_VERSION, ValueObject<JString>(appVersion));
			EGLOG(DebugLevel::INFO, OperationRequest(OperationCode::AUTHENTICATE, op).toString(true));
			return super::opCustom(OperationRequest(OperationCode::AUTHENTICATE, op), true, 0, encrypted);
		}

		bool Peer::opSetPropertiesOfPlayer(int playerNr, const Hashtable& properties)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::PROPERTIES, ValueObject<Hashtable>(properties));
			op.put(Lite::ParameterCode::ACTORNR, ValueObject<nByte>(playerNr));
			
			bool broadcast = true;
			op.put(ParameterCode::BROADCAST, ValueObject<bool>(broadcast));

			return super::opCustom(OperationRequest(OperationCode::SET_PROPERTIES, op), true);
		}

		bool Peer::opSetPropertiesOfRoom(const Hashtable& properties)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::PROPERTIES, ValueObject<Hashtable>(properties));

			bool broadcast = true;
			op.put(ParameterCode::BROADCAST, ValueObject<bool>(broadcast));

			return super::opCustom(OperationRequest(OperationCode::SET_PROPERTIES, op), true);
		}

		Hashtable Peer::stripToCustomProperties(const Hashtable& properties)
		{
			Hashtable retVal;
			const JVector<Object>& keys = properties.getKeys();
			for(unsigned int i=0 ; i<keys.getSize() ; i++)
				if(keys[i].getType() == Common::TypeCode::STRING)
					retVal.put(keys[i], properties[i]);
			return retVal;
		}
		
		Hashtable Peer::stripKeysWithNullValues(const Hashtable& orig)
		{
			Hashtable stripped;
			const JVector<Object>& keys = orig.getKeys();
			for(unsigned int i=0; i<orig.getSize(); i++)
				if(orig[i] != Object())
					stripped.put(keys[i], orig[i]);
			return stripped;
		}

		OperationRequestParameters Peer::opCreateRoomImplementation(const JString& gameID, bool isVisible, bool isOpen, nByte maxPlayers, const Hashtable& customRoomProperties, const Hashtable& customLocalPlayerProperties, const JVector<JString>& propsListedInLobby)
		{
			OperationRequestParameters op;
			if(gameID.length())
				op.put((nByte)ParameterCode::ROOM_NAME, ValueObject<JString>(gameID));

			Hashtable roomProps(stripToCustomProperties(customRoomProperties));
			if(!isOpen)
				roomProps.put(static_cast<nByte>(Properties::Room::IS_OPEN), isOpen);
			if(!isVisible)
				roomProps.put(static_cast<nByte>(Properties::Room::IS_VISIBLE), isVisible);
			if(maxPlayers)
				roomProps.put(static_cast<nByte>(Properties::Room::MAX_PLAYERS), maxPlayers);
			JString* propsListedInLobbyArr = allocateArray<JString>(propsListedInLobby.getSize());
			for(unsigned int i=0; i<propsListedInLobby.getSize(); ++i)
				propsListedInLobbyArr[i] = propsListedInLobby[i];
			roomProps.put(static_cast<nByte>(Properties::Room::PROPS_LISTED_IN_LOBBY), propsListedInLobbyArr, propsListedInLobby.getSize());
			deallocateArray(propsListedInLobbyArr);
			op.put(static_cast<nByte>(ParameterCode::ROOM_PROPERTIES), ValueObject<Hashtable>(roomProps));

			Hashtable playerProperties = stripToCustomProperties(customLocalPlayerProperties);
			if(playerProperties.getSize())
				op.put(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES), ValueObject<Hashtable>(playerProperties));
			op.put(static_cast<nByte>(ParameterCode::BROADCAST), ValueObject<bool>(true));
			op.put(static_cast<nByte>(ParameterCode::CLEANUP_CACHE_ON_LEAVE), ValueObject<bool>(true));
			return op;
		}

		OperationRequestParameters Peer::opJoinRoomImplementation(const JString& gameID, const Hashtable& customLocalPlayerProperties)
		{
			OperationRequestParameters op;
			op.put(static_cast<nByte>(ParameterCode::ROOM_NAME), ValueObject<JString>(gameID));
			
			Hashtable playerProps = stripToCustomProperties(customLocalPlayerProperties);
			if(playerProps.getSize())
				op.put(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES), ValueObject<Hashtable>(playerProps));
			op.put(static_cast<nByte>(ParameterCode::BROADCAST), ValueObject<bool>(true));
			return op;
		}
		
		bool Peer::opRaiseEventImplementation(bool reliable, const Hashtable& parameters, nByte eventCode, nByte channelID, int* targetPlayers, short numTargetPlayers, nByte eventCaching, nByte receiverGroup)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::DATA, ValueObject<Hashtable>(parameters));
			op.put(ParameterCode::CODE, ValueObject<nByte>(eventCode));
			
			if(targetPlayers)
				op.put(Lite::ParameterCode::ACTOR_LIST, ValueObject<int*>(targetPlayers, numTargetPlayers));
			else
			{
				if(eventCaching != EventCache::DO_NOT_CACHE)
					op.put(Lite::ParameterCode::CACHE, ValueObject<nByte>(eventCaching));
				if(receiverGroup != ReceiverGroup::OTHERS)
					op.put(Lite::ParameterCode::RECEIVER_GROUP, ValueObject<nByte>(receiverGroup));
			}
			
			return super::opCustom(OperationRequest(OperationCode::RAISE_EVENT, op), reliable, channelID);
		}
	}
}