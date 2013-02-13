/* Exit Games Common - C Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __CONSTANTS_H 
	#define __CONSTANTS_H

	#if !defined _EG_MARMALADE_PLATFORM && !defined _EG_IPHONE_PLATFORM && !defined _EG_IMAC_PLATFORM && !defined _EG_MEEGO_PLATFORM && !defined _EG_WIN32_PLATFORM && !defined _EG_ANDROID_PLATFORM && !defined _EG_BLACKBERRY_PLATFORM
		#include "platform_definition.h"
	#endif

	#if defined _EG_IPHONE_PLATFORM || defined _EG_IMAC_PLATFORM
		#define _EG_APPLE_PLATFORM true
	#endif

	#if defined _EG_WIN32_PLATFORM
		#define _EG_WINDOWS_PLATFORM true
	#endif

	#if defined _EG_MARMALADE_PLATFORM || defined _EG_APPLE_PLATFORM || defined _EG_MEEGO_PLATFORM || defined _EG_ANDROID_PLATFORM || defined _EG_BLACKBERRY_PLATFORM
		#define _EG_UNIX_PLATFORM true
	#endif

	#if defined _EG_WINDOWS_PLATFORM || (defined _EG_MARMALADE_PLATFORM && defined _MSC_VER && !defined __clang__ && !defined __gcc__ && defined I3D_ARCH_X86)
		#define _EG_MS_COMPILER
	#endif

	#ifndef __cplusplus
		#define false 0
		#define true 1
	#endif

	#ifndef _nByte_defined_
		#define _nByte_defined_
		typedef unsigned char nByte; // unsigned 8  bit value type.
	#endif



	// To activate the debug output sent to your SETDEBUG_CB and PHOTONDEBUG_CB functions.
	// #define EG_DEBUGGER

	static const int SHORT_SIZE   = 2;
	static const int INTEGER_SIZE = 4;
	static const int LONG_SIZE    = 8;
	static const int FLOAT_SIZE   = 4;
	static const int DOUBLE_SIZE  = 8;


	// "static" is default for constants in C++, but not in C, so it is needed for C-support here
	static const nByte EG_BYTE               = 'b';
	static const nByte EG_SHORT              = 'k';
	static const nByte EG_INTEGER            = 'i';
	static const nByte EG_LONG               = 'l';
	static const nByte EG_FLOAT              = 'f';
	static const nByte EG_DOUBLE             = 'd';
	static const nByte EG_BOOLEAN            = 'o';
	static const nByte EG_STRING             = 's';
	static const nByte EG_HASHTABLE          = 'h';
	static const nByte EG_DICTIONARY         = 'D';
	static const nByte EG_OBJECT             = 'z'; // for EG_Object (C) / Object (C++) / NSObject (objC), only allowed for arrays!
	static const nByte EG_ARRAY              = 'y'; // internal only
	static const nByte EG_INTERNAL_BYTEARRAY = 'x'; // internal only
	static const nByte EG_GPOPERATION        = 'g'; // This is internally used for gp operations.
	static const nByte EG_PHOTON_COMMAND     = 'p'; // This is internally used for photon commands.
	static const nByte EG_NULL               = '*'; // internal only, for adding NULL as a value to a HashTable
	static const nByte EG_CUSTOM             = 'c'; // internal only, for custom data, (de-)serialized by callbacks, implemented by the application
	static const nByte EG_UNKNOWN            = 0  ; // internal only, used instead of EG_OBJECT for values inside an EG_DICTIONARY


	// 24hours in ms
	static const unsigned int EG_TIME_OVERFLOW = 86400000u;
#endif