/* Exit Games Common - C Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_TYPES_H
#define __EG_TYPES_H

#include "porting.h"

#include <assert.h>

typedef struct _EG_Callback   EG_Callback;

typedef void (*PFNNOTIFY)(void* pUser);

#if defined _EG_WINDOWS_PLATFORM
#include <windows.h>

typedef struct _ThreadResponse ThreadResponse;

typedef void (*PFN_TIMER_NOTIFY)(void* pData);

//Descriptor structure for memory stream. For internal using only
typedef struct _MemoryStream
{
	char*		_pBuffer;      //Pointer to data buffer
	unsigned	_iSize;         //Buffer size in bytes
	unsigned	_iCurrPosition; //Current offset in buffer
} MemoryStream;

//Structure is used for data exchange between main and other threads. For internal using only
struct _ThreadResponse
{
	PFNNOTIFY		_callback_func; //Pointer to callback function which called when data from server received
	void*			_pNotifyData;	// User defined data for _callback_func 
	MemoryStream*	_mStream;		//Data received from server
	bool			_bProcessed;    //Status flag. If set to false, call still in progress, true response from server received and handled.
};
#endif

//Structure specifies the data and functions for a callback context. For internal using only
struct _EG_Callback
{
	PFNNOTIFY	pfnNotify;      // Filled by caller
	void*		pNotifyData;    // Filled by caller
};

#endif