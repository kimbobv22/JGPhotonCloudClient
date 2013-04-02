/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_CUSTOM_TYPE_CALLBACK_WRAPPER_H
#define __EG_CUSTOM_TYPE_CALLBACK_WRAPPER_H

#import <Foundation/NSData.h>
#import <Foundation/NSDictionary.h>

#import "EGCustomType.h"
#include "Helpers/CustomTypeCallbackWrapper.h"

class EGCustomTypeCallbackWrapper: public ExitGames::Common::Helpers::CustomTypeCallbackWrapper
{
public:
	EGCustomTypeCallbackWrapper();

	static void initClass(nByte typeCode, Class<EGCustomType> delegate);
	static Class<EGCustomType> getClassForCode(nByte typecode);
	
	static void* cbCalloc(short count, nByte customTypeCode);
	static void cbFree(void* pData, nByte customTypeCode);
	static unsigned int cbSizeOf(nByte customTypeCode);

	//From ExitGames::Common::CustomTypeCallbackWrapper
	virtual void cleanup(void* pData);
	virtual bool compare(void* pData1, void* pData2);
	virtual void duplicate(void* pData, void* retVal);
	virtual void deserialize(nByte* pData, short length, void* retVal);
	virtual short serialize(void* pData, nByte* retVal);
	virtual int toString(void* pData, EG_CHAR* buffer);
	virtual ExitGames::Common::Helpers::CustomTypeCallbackWrapper* copy() const;

private:
	static Class<EGCustomType> mDelegates[UCHAR_MAX];
	static NSMutableDictionary* mAllocationSizes;
};

#endif