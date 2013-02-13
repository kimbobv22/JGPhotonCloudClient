/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __PHOTON_PEER_H
#define __PHOTON_PEER_H

#include "PhotonListener.h"
#include "OperationRequest.h"

namespace ExitGames
{
	namespace Photon
	{
		class PhotonPeer
		{
		public:
			PhotonPeer(PhotonListener* listener, bool useTcp=false);
			virtual ~PhotonPeer(void);

			virtual bool connect(const Common::JString& ipAddr, const nByte appID[APP_NAME_LENGTH]=NULL);
			virtual void disconnect(void);
			virtual void service(bool dispatchIncomingCommands=true);
			virtual void serviceBasic(void);
			virtual bool opCustom(const OperationRequest& operationRequest, bool sendReliable, nByte channelID=0, bool encrypt=false);
			virtual void sendOutgoingCommands(void);
			virtual bool dispatchIncomingCommands(void);
			virtual bool establishEncryption(void);
			virtual void fetchServerTimestamp(void);

			const PhotonBaseListener* getListener(void) const;
			int getServerTimeOffset(void) const;
			int getServerTime(void) const;
			int getBytesOut(void) const;
			int getBytesIn(void) const;
			PeerState getPeerState(void) const;
			int getSentCountAllowance(void) const;
			void setSentCountAllowance(int setSentCountAllowance);
			int getTimePingInterval(void) const;
			void setTimePingInterval(int setTimePingInterval);
			int getRoundTripTime(void) const;
			int getRoundTripTimeVariance(void) const;
			PhotonPeer_DebugLevel getDebugOutputLevel(void) const;
			bool setDebugOutputLevel(PhotonPeer_DebugLevel debugLevel);
			int getIncomingReliableCommandsCount(void) const;
			short getPeerId(void) const;
			int getSentTimeAllowance(void) const;
			void setSentTimeAllowance(int setSentTimeAllowance);
			int getQueuedIncomingCommands(void) const;
			int getQueuedOutgoingCommands(void) const;
			Common::JString getServerAddress(void) const;
			bool getIsEncryptionAvailable(void) const;
			static short getPeerCount(void);

			// internal debugging API, use macros below as public interface
			void sendDebugOutput(PhotonPeer_DebugLevel debugLevel, const EG_CHAR* const file, const EG_CHAR* const function, unsigned int line, const EG_CHAR* const dbgMsg, ...);
			void sendDebugOutput(PhotonPeer_DebugLevel debugLevel, const EG_CHAR* const file, const EG_CHAR* const function, unsigned int line, const EG_CHAR* const dbgMsg, va_list args);
		protected:
			SPhotonPeer* m_pPhotonPeer;
		private:
			PhotonListener* m_pListener;

			static void onOperationResponse(PhotonPeer* pPeer, COperationResponse* cOperationResponse);
			static void onPeerStatus(PhotonPeer* pPeer, int statusCode);
			static void onEvent(PhotonPeer* pPeer, CEventData* cEventData);
			static void onDebugReturn(PhotonPeer* pPeer, PhotonPeer_DebugLevel debugLevel, EG_CHAR* szDebugString);
		};
	}
}

// public debugging API
#undef PhotonPeer_sendDebugOutput
#undef PhotonPeer_vsendDebugOutput
#ifdef EG_DEBUGGER
#	define PhotonPeer_sendDebugOutput(peer, debugLevel, ...) (peer)->sendDebugOutput(debugLevel, __WFILE__, __WFUNCTION__, __LINE__, __VA_ARGS__)
#	define PhotonPeer_vsendDebugOutput(peer, debugLevel, dbgMsg, args) (peer)->sendDebugOutput(debugLevel, __WFILE__, __WFUNCTION__, __LINE__, dbgMsg, args)
#else
#	define PhotonPeer_sendDebugOutput(peer, debugLevel, ...) ((void)0)
#	define PhotonPeer_vsendDebugOutput(peer, debugLevel, dbgMsg, args) ((void)0)
#endif

#endif