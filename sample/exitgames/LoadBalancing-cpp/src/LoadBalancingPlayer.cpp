/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2012 by Exit Games. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancingPlayer.h"
#include "LoadBalancingRoom.h"
#include "Internal/Enums/Properties/Player.h"

/** @file LoadBalancingPlayer.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;

		Player::Player(void)
			: mNumber(-1)
		{
		}

		Player::Player(int number, const Hashtable& properties, const MutableRoom* const room)
			: mNumber(number), mRoom(room)
		{
			cacheProperties(properties);
		}

		Player::~Player(void)
		{
		}

		Player::Player(const Player& toCopy)
		{
			*this = toCopy;
		}

		Player& Player::operator=(const Player& toCopy)
		{
			return assign(toCopy);
		}

		Player& Player::assign(const Player& toCopy)
		{
			mNumber = toCopy.mNumber;
			mName = toCopy.mName;
			mCustomProperties = toCopy.mCustomProperties;
			mRoom = toCopy.mRoom;
			return *this;
		}

		int Player::getNumber(void) const
		{
			return mNumber;
		}

		const JString& Player::getName(void) const
		{
			return mName;
		}

		const Hashtable& Player::getCustomProperties(void) const
		{
			return mCustomProperties;
		}

		void Player::cacheProperties(const Hashtable& properties)
		{
			if(properties.contains(static_cast<nByte>(Properties::Player::PLAYERNAME)))
				mName = ValueObject<JString>(properties.getValue(static_cast<nByte>(Properties::Player::PLAYERNAME))).getDataCopy();

			mCustomProperties.put(Peer::stripToCustomProperties(properties));
			mCustomProperties = Peer::stripKeysWithNullValues(mCustomProperties);
		}

		bool Player::operator==(const Player& player) const
		{
			return getNumber() == player.getNumber();
		}

		JString& Player::toString(JString& retStr, bool withTypes) const
		{
			return retStr += toString(withTypes, false);
		}

		JString Player::toString(bool withTypes, bool withCustomProperties) const
		{
			return JString() + mNumber + L"={" + payloadToString(withCustomProperties, withTypes) + L"}";
		}

		JString Player::payloadToString(bool withTypes, bool withCustomProperties) const
		{
			JString res = JString(L"num: ") + mNumber + L" name: " + mName;
			if(withCustomProperties && mCustomProperties.getSize())
				res += L" props: " + mCustomProperties.toString(withTypes);
			return res;
		}



		MutablePlayer::MutablePlayer(int number, const Hashtable& properties, const MutableRoom* const room, Peer* peer)
			: Player(number, properties, room)
			, mLoadBalancingPeer(peer)
		{
		}

		MutablePlayer::~MutablePlayer(void)
		{
		}

		MutablePlayer::MutablePlayer(const MutablePlayer& toCopy) : Player(toCopy)
		{
			*this = toCopy;
		}

		MutablePlayer& MutablePlayer::operator=(const MutablePlayer& toCopy)
		{
			return assign(toCopy);
		}

		MutablePlayer& MutablePlayer::assign(const Player& toCopy)
		{
			super::assign(toCopy);
			mLoadBalancingPeer = dynamic_cast<const MutablePlayer*>(&toCopy)->mLoadBalancingPeer;
			return *this;
		}

		void MutablePlayer::mergeCustomProperties(const Hashtable& customProperties)
		{
			Hashtable stripDict = Peer::stripToCustomProperties(customProperties);
			if(!stripDict.getSize())
				return;
			Hashtable oldDict = mCustomProperties;
			mCustomProperties.put(stripDict);
			mCustomProperties = Peer::stripKeysWithNullValues(mCustomProperties);
			if(mCustomProperties != oldDict)
				mLoadBalancingPeer->opSetPropertiesOfPlayer(mNumber, stripDict);
		}

		void MutablePlayer::addCustomProperties(const Hashtable& customProperties)
		{
			mergeCustomProperties(Peer::stripKeysWithNullValues(customProperties));
		}

		void MutablePlayer::setName(const JString& name)
		{
			if(mName != name)
			{
				Hashtable properties;
				properties.put(static_cast<nByte>(Properties::Player::PLAYERNAME), mName=name);
				mLoadBalancingPeer->opSetPropertiesOfPlayer(mNumber, properties);
			}
		}

		bool MutablePlayer::getIsMasterClient(void) const
		{
			return mRoom?mNumber==mRoom->getMasterClientID():false;
		}
	}
}