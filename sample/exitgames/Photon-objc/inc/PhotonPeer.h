/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __PHOTON_PEER_H
#define __PHOTON_PEER_H

#include "Photon.h"
#import "PhotonListener.h"
#import "OperationRequest.h"
#import "Base.h"
#import "EGTime.h"
#import "NSString+UTF32.h"
#import "EGDictionary.h"
#import "NSObject+CopyDeep.h"
#import "CustomTypeUtils.h"

@interface PhotonPeer : NSObject
{
@protected
	SPhotonPeer* m_pPhotonPeer;
@private
	id<PhotonListener> m_plistener;
}

@property (readonly) id<BaseListener> Listener;
@property (readonly) int ServerTimeOffset;
@property (readonly) int ServerTime;
@property (readonly) int BytesOut;
@property (readonly) int BytesIn;
@property (readonly) PeerState Peerstate;
@property (readwrite) int SentCountAllowance;
@property (readwrite) int TimePingInterval;
@property (readonly) int RoundTripTime;
@property (readonly) int RoundTripTimeVariance;
@property (readwrite) PhotonPeer_DebugLevel DebugOutputLevel;
@property (readonly) short PeerID;
@property (readonly) bool IsEncryptionAvailable;
@property (readonly) int IncomingReliableCommandsCount;
@property (readwrite) int SentTimeAllowance;
@property (readonly) int QueuedIncomingCommands;
@property (readonly) int QueuedOutgoingCommands;
@property (readonly) const NSString* ServerAddress;
@property (readonly) short PeerCount;

- (id) initWithPhotonListener:(id<PhotonListener>)listener; // = false
- (id) initWithPhotonListener:(id<PhotonListener>)listener :(bool)useTcp;
- (bool) connect:(const NSString* const)ipAddr; // = NULL
- (bool) connect:(const NSString* const)ipAddr :(const nByte[APP_NAME_LENGTH])appID;
- (void) disconnect;
- (void) service; // = true
- (void) service:(bool)dispatchIncomingCommands;
- (void) serviceBasic;
- (bool) opCustom:(const OperationRequest* const)operationRequest :(bool)sendReliable; // = 0, = false
- (bool) opCustom:(const OperationRequest* const)operationRequest :(bool)sendReliable :(nByte)channelID; // = false
- (bool) opCustom:(const OperationRequest* const)operationRequest :(bool)sendReliable :(nByte)channelID :(bool)encrypt;
- (void) sendOutgoingCommands;
- (bool) dispatchIncomingCommands;
- (bool) establishEncryption;
- (void) fetchServerTimestamp;

// internal debugging API, use macros below as public interface
- (void) sendDebugOutput:(PhotonPeer_DebugLevel)debugLevel :(const EG_CHAR* const)file :(const EG_CHAR* const)function :(unsigned int)line :(const NSString* const)dbgMsg, ...;
- (void) sendDebugOutput:(PhotonPeer_DebugLevel)debugLevel :(const EG_CHAR* const)file :(const EG_CHAR* const)function :(unsigned int)line :(const NSString* const)dbgMsg :(va_list)args;
@end

#undef PhotonPeer_sendDebugOutput
#undef PhotonPeer_vsendDebugOutput
#ifdef EG_DEBUGGER
#	define PhotonPeer_sendDebugOutput(peer, debugLevel, ...) [peer sendDebugOutput:debugLevel :__WFILE__ :__WFUNCTION__ :__LINE__ :__VA_ARGS__]
#	define PhotonPeer_vsendDebugOutput(peer, debugLevel, dbgMsg, args) [peer sendDebugOutput:debugLevel :__WFILE__ :__WFUNCTION__ :__LINE__ :dbgMsg :args]
#else
#	define PhotonPeer_sendDebugOutput(peer, debugLevel, ...) ((void)0)
#	define PhotonPeer_vsendDebugOutput(peer, debugLevel, dbgMsg, args) ((void)0)
#endif

#endif