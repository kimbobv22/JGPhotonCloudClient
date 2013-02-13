/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2012 by Exit Games. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancingRoom.h"
#include "Internal/Enums/Properties/Room.h"

/** @file LoadBalancingRoom.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;

		Room::Room(void)
			: mName(JString())
			, mPlayerCount(0)
			, mMaxPlayers(0)
			, mIsOpen(false)
			, mCustomProperties(Hashtable())
		{
		}

		Room::Room(const JString& name, const Hashtable& properties)
			: mName(name)
			, mPlayerCount(0)
			, mMaxPlayers(0)
			, mIsOpen(true)
			, mCustomProperties(Hashtable())
		{
			cacheProperties(properties);
		}

		Room::~Room(void)
		{
		}

		Room::Room(const Room& toCopy)
		{
			*this = toCopy;
		}

		Room& Room::operator=(const Room& toCopy)
		{
			return assign(toCopy);
		}

		Room& Room::assign(const Room& toCopy)
		{
			mName = toCopy.mName;
			mPlayerCount = toCopy.mPlayerCount;
			mMaxPlayers = toCopy.mMaxPlayers;
			mIsOpen = toCopy.mIsOpen;
			mCustomProperties = toCopy.mCustomProperties;
			return *this;
		}

		const Hashtable& Room::getCustomProperties(void) const
		{
			return mCustomProperties;
		}

		const JString& Room::getName(void) const
		{
			return mName;
		}

		nByte Room::getPlayerCount(void) const
		{
			return mPlayerCount;
		}

		nByte Room::getMaxPlayers(void) const
		{
			return mMaxPlayers;
		}

		bool Room::getIsOpen(void) const
		{
			return mIsOpen;
		}

		void Room::cacheProperties(const Hashtable& properties)
		{
			if(properties.contains(static_cast<nByte>(Properties::Room::MAX_PLAYERS)))
				mMaxPlayers = ValueObject<nByte>(properties.getValue(static_cast<nByte>(Properties::Room::MAX_PLAYERS))).getDataCopy();

			if(properties.contains(static_cast<nByte>(Properties::Room::IS_OPEN)))
				mIsOpen = ValueObject<bool>(properties.getValue(static_cast<nByte>(Properties::Room::IS_OPEN))).getDataCopy();

			if(properties.contains(static_cast<nByte>(Properties::Room::PLAYER_COUNT)))
				mPlayerCount = ValueObject<nByte>(properties.getValue(static_cast<nByte>(Properties::Room::PLAYER_COUNT))).getDataCopy();

			mCustomProperties.put(Peer::stripToCustomProperties(properties));
			mCustomProperties = Peer::stripKeysWithNullValues(mCustomProperties);
		}

		bool Room::operator==(const Room& room) const
		{
			return getName() == room.getName();
		}

		JString& Room::toString(JString& retStr, bool withTypes) const
		{
			return retStr += toString(withTypes, false);
		}

		JString Room::toString(bool withTypes, bool withCustomProperties) const
		{
			return mName + L"={" + payloadToString(withCustomProperties) + L"}";
		}

		JString Room::payloadToString(bool withTypes, bool withCustomProperties) const
		{
			JString res = mName + L" pl: " + mPlayerCount + L" max: " + mMaxPlayers + L" open: " + mIsOpen;
			if(withCustomProperties && mCustomProperties.getSize())
				res += L" props:" + mCustomProperties.toString(withTypes);
			return res;
		}



		MutableRoom::MutableRoom(const JString& name, const Hashtable& properties, Peer* peer, const JVector<JString>& propsListedInLobby)
			: super(name, properties)
			, mLoadBalancingPeer(peer)
			, mIsVisible(true)
			, mPlayers(JVector<Player>())
			, mLocalPlayer(createMutablePlayer(-1, Hashtable()))
			, mMasterClientID(0)
			, mPropsListedInLobby(propsListedInLobby)
		{
			cacheProperties(properties);
		}

		MutableRoom::~MutableRoom(void)
		{
		}

		MutableRoom::MutableRoom(const MutableRoom& toCopy)
			: super(toCopy)
			, mLocalPlayer(toCopy.mLocalPlayer)
		{
			*this = toCopy;
		}

		MutableRoom& MutableRoom::operator=(const MutableRoom& toCopy)
		{
			return assign(toCopy);
		}

		MutableRoom& MutableRoom::assign(const Room& toCopy)
		{
			super::assign(toCopy);
			const MutableRoom& temp = *dynamic_cast<const MutableRoom*>(&toCopy);
			mLoadBalancingPeer = temp.mLoadBalancingPeer;
			mIsVisible = temp.mIsVisible;
			mPlayers = temp.mPlayers;
			mLocalPlayer = temp.mLocalPlayer;
			mMasterClientID = temp.mMasterClientID;
			mPropsListedInLobby = temp.mPropsListedInLobby;
			mLoadBalancingPeer = temp.mLoadBalancingPeer;
			return *this;
		}

		void MutableRoom::mergeCustomProperties(const Hashtable& customProperties)
		{
			Hashtable stripDict = Peer::stripToCustomProperties(customProperties);
			if(!stripDict.getSize())
				return;
			Hashtable oldDict = mCustomProperties;
			mCustomProperties.put(stripDict);
			mCustomProperties = Peer::stripKeysWithNullValues(mCustomProperties);
			if(mCustomProperties != oldDict)
				mLoadBalancingPeer->opSetPropertiesOfRoom(stripDict);
		}

		void MutableRoom::addCustomProperties(const Hashtable& customProperties)
		{
			mergeCustomProperties(Peer::stripKeysWithNullValues(customProperties));
		}

		void MutableRoom::cacheProperties(const Hashtable& properties)
		{
			if(properties.contains(static_cast<nByte>(Properties::Room::IS_VISIBLE)))
				mIsVisible = ValueObject<bool>(properties.getValue(static_cast<nByte>(Properties::Room::IS_VISIBLE))).getDataCopy();
			super::cacheProperties(properties);
		}

		JString MutableRoom::toString(bool withTypes, bool withCustomProperties, bool withLocalPlayer, bool withPlayers) const
		{
			return mName + L"={" + payloadToString(withTypes, withCustomProperties, withLocalPlayer, withPlayers) + L"}";
		}

		JString MutableRoom::payloadToString(bool withTypes, bool withCustomProperties, bool withLocalPlayer, bool withPlayers) const
		{
			JString res = JString(L"masterClient: ") + mMasterClientID + L" " + super::payloadToString() + L" visible: " + mIsVisible;
			if(mPropsListedInLobby.getSize())
				res += L" propsListedInLobby: " + mPropsListedInLobby.toString(withTypes);
			if(withCustomProperties && mCustomProperties.getSize())
				res += L" props: " + mCustomProperties.toString(withTypes);
			if(withLocalPlayer)
				res += L" localPlayer: " + mLocalPlayer.toString();
			if(withPlayers && mPlayers.getSize())
			{
				res += L" players: ";
				for(unsigned int i=0; i<mPlayers.getSize(); ++i)
					res += mPlayers[i].toString(withTypes, withCustomProperties) + (i<mPlayers.getSize()-1?L", ":L"");
			}
			return res;
		}

		void MutableRoom::setMaxPlayers(nByte maxPlayers)
		{
			if(mMaxPlayers != maxPlayers)
				setRoomProperty(static_cast<nByte>(Properties::Room::MAX_PLAYERS), mMaxPlayers=maxPlayers);
		}

		void MutableRoom::setIsOpen(bool isOpen)
		{
			if(mIsOpen != isOpen)
				setRoomProperty(static_cast<nByte>(Properties::Room::IS_OPEN), mIsOpen=isOpen);
		}

		bool MutableRoom::getIsVisible(void) const
		{
			return mIsVisible;
		}

		void MutableRoom::setIsVisible(bool isVisible)
		{
			if(mIsVisible != isVisible)
				setRoomProperty(static_cast<nByte>(Properties::Room::IS_VISIBLE), mIsVisible=isVisible);
		}

		template<typename Etype> void MutableRoom::setRoomProperty(nByte key, Etype val)
		{
			Hashtable properties;
			properties.put(key, val);
			mLoadBalancingPeer->opSetPropertiesOfRoom(properties);
		}

		const JVector<Player>& MutableRoom::getPlayers(void) const
		{
			return mPlayers;
		}
		
		const Player* MutableRoom::getPlayerForNumber(int playerNumber) const
		{
			for(unsigned i=0; i<mPlayers.getSize(); i++)
				if(mPlayers[i].getNumber() == playerNumber)
					return &mPlayers[i];
			return NULL;
		}

		void MutableRoom::setPlayers(const JVector<Player>& players)
		{
			mPlayers = players;
		}

		void MutableRoom::setPlayerForNumber(int playerNumber, const Player& player)
		{
			for(unsigned i=0; i<mPlayers.getSize(); i++)
				if(mPlayers[i].getNumber() == playerNumber)
					mPlayers[i] = player;
		}

		void MutableRoom::setPlayerForNumber(int playerNumber, const Hashtable& properties)
		{
			setPlayerForNumber(playerNumber, createPlayer(playerNumber, properties));
		}

		void MutableRoom::removeAllPlayers(void)
		{
			mPlayers.removeAllElements();
		}

		void MutableRoom::addPlayer(const Player& player)
		{
			mPlayers.addElement(player);
			if(!mMasterClientID || player.getNumber() < mMasterClientID)
				mMasterClientID = player.getNumber();
		}

		void MutableRoom::addPlayer(int number, const Hashtable& properties)
		{
			addPlayer(createPlayer(number, properties));
		}

		bool MutableRoom::removePlayer(const Player& player)
		{
			bool retVal = mPlayers.removeElement(player);
			if(player.getNumber() == mMasterClientID)
			{
				mMasterClientID = mPlayers.getSize()?mPlayers[0].getNumber():0;
				for(unsigned int i=1; i<mPlayers.getSize(); ++i)
					if(mPlayers[i].getNumber() < mMasterClientID)
						mMasterClientID = mPlayers[i].getNumber();
			}
			return retVal;
		}

		bool MutableRoom::removePlayer(int number)
		{
			return removePlayer(createPlayer(number, Hashtable()));
		}

		MutablePlayer& MutableRoom::getLocalPlayer(void)
		{
			return mLocalPlayer;
		}

		void MutableRoom::setLocalPlayer(const MutablePlayer& player)
		{
			mLocalPlayer = player;
		}

		void MutableRoom::setLocalPlayer(int number, const Hashtable& properties)
		{
			setLocalPlayer(createMutablePlayer(number, properties));
		}

		int MutableRoom::getMasterClientID(void) const
		{
			return mMasterClientID;
		}

		const JVector<JString>& MutableRoom::getPropsListedInLobby(void) const
		{
			return mPropsListedInLobby;
		}

		void MutableRoom::setPropsListedInLobby(const JVector<JString>& propsListedInLobby)
		{
			if(mPropsListedInLobby != propsListedInLobby)
			{
				mPropsListedInLobby = propsListedInLobby;
				JString* propsListedInLobbyArr = new JString[propsListedInLobby.getSize()];
				for(unsigned int i=0; i<propsListedInLobby.getSize(); ++i)
					propsListedInLobbyArr[i] = propsListedInLobby[i];
				Hashtable properties;
				properties.put(static_cast<nByte>(Properties::Room::PROPS_LISTED_IN_LOBBY), propsListedInLobbyArr, propsListedInLobby.getSize());
				delete[] propsListedInLobbyArr;
				mLoadBalancingPeer->opSetPropertiesOfRoom(properties);
			}
		}

		Player MutableRoom::createPlayer(int number, const Hashtable& properties) const
		{
			return Player(number, properties, this);
		}

		MutablePlayer MutableRoom::createMutablePlayer(int number, const Hashtable& properties) const
		{
			return MutablePlayer(number, properties, this, mLoadBalancingPeer);
		}
	}
}