/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __CUSTOM_TYPE_CALLBACK_WRAPPER_H
#define __CUSTOM_TYPE_CALLBACK_WRAPPER_H

#import <Foundation/NSData.h>
#import <Foundation/NSDictionary.h>
#include "data_structures.h"
#import <CustomType.h>

class CustomTypeCallbackWrapper
{
public:
	static void initClass(nByte typeCode, Class<CustomType> delegate);
	static Class<CustomType> getClassForCode(nByte typecode);
	
	static void ccleanup(void* pData);
	static bool ccompare(void* pData1, void* pData2);
	static void cduplicate(void* pData, void* retVal);
	static void cdeserialize(nByte* pData, short length, void* retVal);
	static short cserialize(void* pData, nByte* retVal);
	static int ctoString(void* pData, EG_CHAR* buffer);
	static void* cbCalloc(short count, nByte customTypeCode);
	static void cbFree(void* pData, nByte customTypeCode);
	static unsigned int cbSizeOf(nByte customTypeCode);

private:
	static Class<CustomType> mDelegates[UCHAR_MAX];
	static NSMutableDictionary* mAllocationSizes;
};

#endif