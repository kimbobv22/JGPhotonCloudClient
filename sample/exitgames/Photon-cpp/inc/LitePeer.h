/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __LITE_PEER_H
#define __LITE_PEER_H

#include "PhotonPeer.h"

namespace ExitGames
{
	namespace Photon
	{
		class LitePeer : public PhotonPeer
		{
		public:
			LitePeer(PhotonListener* listener, bool useTcp=false);
			virtual ~LitePeer(void);
			virtual bool opRaiseEvent(bool sendReliable, const Common::Hashtable& evData, nByte eventCode, nByte channelID=0, int* targetActors=NULL, short numTargetActors=0);
			virtual bool opRaiseEvent(bool sendReliable, const Common::Hashtable& evData, nByte eventCode, nByte channelID, nByte eventCaching, nByte receiverGroup=RG_OTHERS);
			virtual bool opJoin(const Common::JString& gameId, const Common::Hashtable& gameProperties=Common::Hashtable(), const Common::Hashtable& actorProperties=Common::Hashtable(), bool broadcastActorProperties=false);
			virtual bool opLeave(const Common::JString& gameId);
			virtual bool opSetPropertiesOfActor(int actorNr, const Common::Hashtable& properties, bool broadcast, nByte channelID=0);
			virtual bool opSetPropertiesOfGame(const Common::Hashtable& properties, bool broadcast, nByte channelID=0);
			virtual bool opGetProperties(nByte channelID=0);
			virtual bool opGetPropertiesOfActor(const Common::JString* const properties, short numProperties, const int* const actorNrList=NULL, short numActors=0, nByte channelID=0);
			virtual bool opGetPropertiesOfActor(const nByte* const properties, short numProperties, const int* const actorNrList=NULL, short numActors=0, nByte channelID=0);
			virtual bool opGetPropertiesOfGame(const Common::JString* const properties, short numProperties, nByte channelID=0);
			virtual bool opGetPropertiesOfGame(const nByte* const properties, short numProperties, nByte channelID=0);
		};
	}
}

#endif