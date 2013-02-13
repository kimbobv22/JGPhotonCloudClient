/* Exit Games Photon - C Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __CALLBACKS_H
#define __CALLBACKS_H

//********************************************************************************************************************************
// Callback typedef functions
//********************************************************************************************************************************

/**
   Typedef for the call back function to be invoked in response
   to basic operations sent to the Photon Server providing the result values from the server.
   @details
   This CB is used as general callback for all operations. The type of an operation is
   identified by an opCode.

   An operation's result is summarized by the returnCode: an int typed code, 0 for OK or
   some error code defined by the application, which is defining the operation itself.
   The opCode defines the type of operation called on Photon and in turn also the returnValues. They are
   provided as Hashtable which contains the complete answer of Photon, including keys for
   opCode and returnCode. Each operation returns its opCode and returnCode but anything else can be
   defined serverside.

   @param pUser user data
   @param operationResponse the operationResponse                       */
typedef void(*CB_ON_OPERATION_RESPONSE)(void* pUser, COperationResponse* operationResponse);

/** 
   CB_ON_STATUS_CHANGED is used to denote errors or simply state-changes of the respective PhotonPeer.
   @details
   State change callback

   When this function is used to signalize a state-change, the opCode and other values will be 0 (or null)
   with the following statusCode values:
    * SC_CONNECT the connection to the Photon Server was established
    * SC_DISCONNECT the connection was closed (due to an API-call or a timeout)
    * SC_EXCEPTION/SC_EXCEPTION_CONNECT some system or network error closed the connection

   Possible errors
   When developing with the Photon API, hundreds of events might reach a client each minute. If they are
   not processed in time, they might cause issues. As result of this, there are warnings and errors,
   which should help getting the timing right. Check PhotonConstants for a list (they start with SC_).

   @param pUser user data
   @param statusCode see @details                        */
typedef void(*CB_ON_STATUS_CHANGED)(void* pUser, int statusCode);

/**
   Typedef for the event handler function for all Neutron Events
   transmitted by PhotonPeer (UDP).
   @details
   Whenever a Photon event is sent and received via UDP, the
   local peer will be notified via this function. So this is the
   corresponding callback function to Neutron's EVENTACTION_CB
   for HTTP transmissions. Please refer to "sending and
   receiving data" for more information. When a new PhotonPeer is
   created, a pointer to a callback function must be set using
   the function:
   
   PhotonPeer_SetCBOnEvent()
   
   This way, an application can react on any event, based on its
   eventCode.
   
   The following Neutron events are reported by default:
   
   EV_RT_JOIN
   
   EV_RT_LEAVE
   
   These events are predefined and will be triggered as soon as
   a player has joined or has left the room the local player is
   currently active in. To transmit in-room data, define your
   \own events as needed for your application, and transmit them
   using PhotonPeer_opRaiseEvent().
   
   
   
   All events which are raised in reaction to some player's
   actions (like sending data), contain the actor number of the
   sending player in the "parameters" Hashtable. To extract
   this actor number from an event (where available) use code
   like this:
   
   <code>
   nByte key = EV_RT_KEY_ACTORNR;
   \*(int*)( EG_HashTable_getValFromKey(parameters, &amp;key, EG_BYTE, NULL )-\>obj_data );
   
   </code>
   
   If the received event has been raised by another player by
   calling PhotonPeer_opRaiseEvent(), the transmitted "evData"
   hashtable will be stored in the "parameters" hashtable of
   CB_ON_EVENT at key EV_RT_KEY_DATA. Please refer to the
   "realtime demo" in your SDK for sample code.
   @param pUser user data
   @param eventDataContent the event
   @sa
   <link MemoryManagement, Sending and receiving data> , PhotonPeer_opRaiseEvent() */
typedef void(*CB_ON_EVENT)(void* pUser, CEventData* eventDataContent);

/**
   Typedef for a callback function used for receiving debug
   messages generated by Photon functions.
   @details
   Please refer to PhotonPeer_SetCBDebugReturn()
   @param pUser user data
   @param debugLevel the minimum PhotonPeer_DebugLevel value needed for this message to be passed to your application
   @param debugString string containing debug information                                                      */
typedef void(*CB_DEBUG_RETURN) (void* pUser, PhotonPeer_DebugLevel debugLevel, EG_CHAR* debugString);

#endif