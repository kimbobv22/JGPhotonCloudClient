/* Exit Games Common - C Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __DEFINES_H
#define __DEFINES_H

#include "Constants.h"

#ifndef _INT64
#define _INT64
typedef  signed long long   int64;       /* Signed 64 bit value */
typedef  unsigned long long uint64;      /* Unsigned 64 bit value */
#endif

// UTF8
#define UTF8LEN(str) utf8len(str)

// Definitions for memory management //////////////////////////////////////
#include <memory.h>
#include <string.h>
#define MEMCPY(dst, src, size)	memcpy(dst, src, size)
#define MEMSET(dst, c, size)	memset(dst, c, size)
#define ZEROAT(ptr)				(void)MEMSET(ptr, 0, sizeof(*ptr))

// Definitions for memory allocation /////////////////////////////////////
#ifdef _EG_APPLE_PLATFORM
#	include <stdlib.h>
#else
#	include <malloc.h>
#endif

static __inline void* __MALLOC(unsigned int size)
{
	// this inline function is needed for clang static analyzer, which is wrongly detecting a memleak for every call to MALLOC, when simply defining MALLOC as memset(malloc(size), 0, size)
	void* retVal = malloc(size);
	memset(retVal, 0, size);
	return retVal;
}

#define MALLOC                  __MALLOC
#define FREE(p)                 ((p)?(free(p), (p)=NULL):(void*)0)
#define REALLOC                 realloc

// Definitions for string operations /////////////////////////////////////
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

/* ATTENTION:
wchar_t is UTF16 on Windows, but UTF32 on UNIX and it's inheritors OSX/iOS, although OSX/iOS-NSString/CFString uses UTF16,
so you have to convert EG_CHAR to/from NSString/CFString, but redefining wchar_t to be 16bit on UNIX or defining EG_CHAR to be always
16bit would have meant reimplementing standard wchar_t library on UNIX */
typedef wchar_t EG_CHAR;

#ifdef  __cplusplus
extern "C"
{
#endif
	EG_CHAR* ANSI2Unicode(const char* _str, EG_CHAR* _wstr, unsigned long _wstrLen);
	char* Unicode2ANSI(const EG_CHAR* _wstr, char* _str, unsigned long _strSize);
	char* Unicode2UTF8(const EG_CHAR* wstr, unsigned long src_len, char* dst, unsigned long dst_size);
	EG_CHAR* UTF82Unicode(const unsigned char* str, unsigned long src_size, EG_CHAR* dst, unsigned long dst_len);
	int SizeOfWStrAsUTF8(const EG_CHAR* str, unsigned long len);
	int LengthOfUTF8AsWStr(const unsigned char* str, unsigned long size);
#ifdef  __cplusplus
}
#endif

// Unicode string support and conversion
#define WSTRTOSTR(src, dst, size)               Unicode2ANSI(src, dst, size)
#define STRTOWSTR(src, dst, size)               ANSI2Unicode(src, dst, size)
#define WSTRTOUTF8(src, src_len, dst, dst_size) Unicode2UTF8(src, src_len, dst, dst_size)
#define UTF8TOWSTR(src, src_size, dst, dst_len) UTF82Unicode(src, src_size,  dst, dst_len)

// ASCII
#define STRCMP(str1, str2)                  strcmp(str1, str2)
#define STRCAT(dst, src)                    strcat(dst, src)
#define STRSTR(str1, str2)                  strstr(str1, str2)
#define STRLEN(str)                         strlen(str)
#define STRNLEN(str,n)                      strnlen(str,n)
#define STRCPY(dst, src)                    strcpy(dst, src)
#define STRCHR(str, c)                      strchr(str, c)
#define STRRCHR(str, c)                     strrchr(str, c)
#define STRNCMP(str1, str2, c)              strncmp(str1, str2,
#if defined _EG_WINDOWS_PLATFORM
#	define STRLOWER(str)                    _strlwr(str)
#	define STRDUP(str)                      _strdup(str)c)
#	define STRNICMP(str1, str2, c)          _strnicmp(str1, str2, c)
#	define VSNPRINTF                        _vsnprintf
#	define SNPRINTF                         _snprintf
#else
#	define STRLOWER(str)                    strlwr(str)
#	define STRDUP(str)                      strdup(str)
#	define STRNICMP(str1, str2, c)          EG_strnicmp(str1, str2, c)
#	define VSNPRINTF                        vsnprintf
#	define SNPRINTF                         snprintf
#endif
#define SPRINTF                             sprintf

// UTF16 / UTF32
#if defined _EG_ANDROID_PLATFORM
#	define WCSCMP(str1, str2)               EG_wcscmp(str1, str2)
#	define WCSCAT(dst, src)                 EG_wcscat(dst, src)
#	define WCSSTR(str1, str2)               EG_wcsstr(str1, str2)
#	define WCSCPY(dst, src)                 EG_wcscpy(dst, src)
#	define WCSLEN(str)                      EG_wcslen(str)
#	define WCSNLEN(str,n)                   EG_wcsnlen(str,n)
#	define WCSCHR(str, c)                   EG_wcschr(str, c)
#	define WCSRCHR(str, c)                  EG_wcsrchr(str, c)
#	define WCSNCMP(str1, str2, c)           EG_wcsncmp(str1, str2, c)
#else
#	define WCSCMP(str1, str2)               wcscmp(str1, str2)
#	define WCSCAT(dst, src)                 wcscat(dst, src)
#	define WCSSTR(str1, str2)               wcsstr(str1, str2)
#	define WCSCPY(dst, src)                 wcscpy(dst, src)
#	define WCSLEN(str)                      wcslen(str)
#	define WCSNLEN(str,n)                   wcsnlen(str,n)
#	define WCSLOWER(str)                    _wcslwr(str)
#	define WCSCHR(str, c)                   wcschr(str, c)
#	define WCSRCHR(str, c)                  wcsrchr(str, c)
#	define WCSNCMP(str1, str2, c)           wcsncmp(str1, str2, c)
#	define WCSNICMP(str1, str2, c)          _wcsnicmp(str1, str2, c)
#	define WCSDUP(str)                      _wcsdup(str)
#endif
#define VWSCPRINTF                          _vwscprintf
#if defined _EG_WINDOWS_PLATFORM
#	define VSNWPRINTF(dst, cnt, frmt, args) _vsnwprintf_s(dst, cnt, cnt-1, frmt, args)
#elif defined _EG_MARMALADE_PLATFORM && defined I3D_ARCH_ARM || defined _EG_ANDROID_PLATFORM
#	define VSNWPRINTF                       EG_vswprintf // very expensive, use with care!
#else
#	define VSNWPRINTF                       vswprintf
#endif
#define VSCWPRINTF                           _vscwprintf
#if defined _EG_WINDOWS_PLATFORM
#	define SNWPRINTF                        _snwprintf
#elif defined _EG_MARMALADE_PLATFORM && defined I3D_ARCH_ARM || defined _EG_ANDROID_PLATFORM
#	define SNWPRINTF                        EG_swprintf
#else
#	define SNWPRINTF                        swprintf
#endif

#define ATOI(str)                            atoi(str)
#if defined _EG_WINDOWS_PLATFORM
#	define WTOI(str)                        _wtoi(str)
#else
#	define WTOI(str)                        EG_wtoi(str)
#endif

#define CUTPATH(file) (STRRCHR(file, '/')?STRRCHR(file, '/')+1:STRRCHR(file, '\\')?STRRCHR(file, '\\')+1:file)
#define WCUTPATH(file) (WCSRCHR(file, '/')?WCSRCHR(file, '/')+1:WCSRCHR(file, '\\')?WCSRCHR(file, '\\')+1:file)

// widestring versions of predefined macros
// __WFILE__ and __WFUNCTION__ instead of __FILEW__ and __FUNCTIONW__, to avoid conflicts with MS sytem libs, which do not #ifdef their defines
#ifndef WIDEN
#	define WIDEN2(str) (L ## str)
#	define WIDEN(str) WIDEN2(str)
#endif
#ifndef __WFILE__
#	define __WFILE__ WIDEN(__FILE__)
#endif
#ifndef __WFUNCTION__
#	if defined _MSC_VER && !defined __clang__ && !defined __gcc__// Marmalade really even defines _MSC_VER, when using another compiler, so we have to additionally check, that no other (of the used by Marmalade) compilers is running
#		define __WFUNCTION__ WIDEN(__FUNCTION__)
#	else
		extern EG_CHAR __EGGlobalwstr[1024];
#		define __WFUNCTION__ STRTOWSTR(__FUNCTION__, __EGGlobalwstr, STRLEN(__FUNCTION__)+1) // we can't use preprocessor token pasting here, as on GCC __FUNCTION__ is not a macro like in VS
#	endif
#endif

// format specifiers:
#define FRMT_SPCFR_STRTOSTR_A "s"
#define FRMT_SPCFR_WSTRTOSTR_A "ls" // "S" would also work
#if defined _EG_WINDOWS_PLATFORM || (defined _EG_MARMALADE_PLATFORM && !defined I3D_ARCH_ARM)
#	define FRMT_SPCFR_STRTOWSTR_A "S"
#else
#	define FRMT_SPCFR_STRTOWSTR_A "s"
#endif
#define FRMT_SPCFR_WSTRTOWSTR_A "ls"

#define FRMT_SPCFR_STRTOSTR_W L"s"
#define FRMT_SPCFR_WSTRTOSTR_W L"ls" // "S" would also work
#if defined _EG_WINDOWS_PLATFORM || (defined _EG_MARMALADE_PLATFORM && !defined I3D_ARCH_ARM)
#	define FRMT_SPCFR_STRTOWSTR_W L"S"
#else
#	 define FRMT_SPCFR_STRTOWSTR_W L"s"
#endif
#define FRMT_SPCFR_WSTRTOWSTR_W L"ls"

// Time access and control functions //////////////////////////////////////

// Attention: Do not use these macros, if you need the absolute time on different platforms, because they have completely different base dates
// on different platforms and are only to be used for time intervals between two calls

#include <time.h>
#if defined _EG_WINDOWS_PLATFORM
#	if defined _EG_WIN32_PLATFORM
#		define WIN32_LEAN_AND_MEAN
#		define _WIN32_WINNT 0x0500
#		include <Windows.h>
#		include <Mmsystem.h>
#	endif
#   define GETTIMEMS	timeGetTime // returns the number of milliseconds for which the PC was powered on - accuracy: 1ms - overhead: medium
#   define GETUPTIMEMS	GetTickCount // returns the number of milliseconds for which the PC was powered on - accuracy: 50ms - overhead. small
#elif defined _EG_UNIX_PLATFORM
#   define GETTIMEMS	getTimeUnix // returns the number of milliseconds passed since 1970 (overflowing all about 49 days!)
#   define GETUPTIMEMS	getTimeUnix // returns the number of milliseconds passed since 1970 (overflowing all about 49 days!)
#endif

// Debug output functions ////////////////////////////////////////////////
#if defined _EG_MEEGO_PLATFORM || defined _EG_BLACKBERRY_PLATFORM
	#include "stdarg.h"
#endif
#if(DBGPRINTF_LEVEL || DBGPRINTF_MEMORY_ACTIVE || DBGPRINTF_PERFORMANCE_ACTIVE)

#if(defined _EG_WINDOWS_PLATFORM) && defined __cplusplus
#	pragma managed(push, off)
#endif
#ifdef __cplusplus
	extern "C"
	{
#endif
		static __inline void __DBGPRINTF(const char* message, ...)
		{
			va_list argptr;
			va_start(argptr, message);
			vfprintf(stderr, message, argptr);
			va_end(argptr);
			fprintf(stderr, "\n");
		}
#ifdef  __cplusplus
	}
#endif
#if(defined _EG_WINDOWS_PLATFORM) && defined __cplusplus
#	pragma managed(pop)
#endif

#define DBGPRINTF_LEVEL_DEBUG_VERBOSE 6
#define DBGPRINTF_LEVEL_DEBUG         5
#define DBGPRINTF_LEVEL_INFO          4
#define DBGPRINTF_LEVEL_WARNING       3
#define DBGPRINTF_LEVEL_ERROR         2
#define DBGPRINTF_LEVEL_FATAL         1
#define DBGPRINTF_LEVEL_OFF           0

#define DBGPRINTF_EXSTR_FORMAT "%13s: %-30s %-50s line: %5u - "

#if(DBGPRINTF_LEVEL >= DBGPRINTF_LEVEL_DEBUG_VERBOSE)
#	define DBGPRINTF_DEBUG_VERBOSE      fprintf(stderr, DBGPRINTF_EXSTR_FORMAT, "DEBUG_VERBOSE", CUTPATH(__FILE__), __FUNCTION__, __LINE__), __DBGPRINTF
#else
#	define DBGPRINTF_DEBUG_VERBOSE(...) ((void)0)
#endif

#if(DBGPRINTF_LEVEL >= DBGPRINTF_LEVEL_DEBUG)
#	define DBGPRINTF_DEBUG              fprintf(stderr, DBGPRINTF_EXSTR_FORMAT, "DEBUG", CUTPATH(__FILE__), __FUNCTION__, __LINE__), __DBGPRINTF
#else
#	define DBGPRINTF_DEBUG(...)         ((void)0)
#endif

#if(DBGPRINTF_LEVEL >= DBGPRINTF_LEVEL_INFO)
#	define DBGPRINTF_INFO               fprintf(stderr, DBGPRINTF_EXSTR_FORMAT, "INFO", CUTPATH(__FILE__), __FUNCTION__, __LINE__), __DBGPRINTF
#else
#	define DBGPRINTF_INFO(...)          ((void)0)
#endif

#if(DBGPRINTF_LEVEL >= DBGPRINTF_LEVEL_WARNING)
#	define DBGPRINTF_WARNING            fprintf(stderr, DBGPRINTF_EXSTR_FORMAT, "WARNING", CUTPATH(__FILE__), __FUNCTION__, __LINE__), __DBGPRINTF
#else
#	define DBGPRINTF_WARNING(...)       ((void)0)
#endif

#if(DBGPRINTF_LEVEL >= DBGPRINTF_LEVEL_ERROR)
#	define DBGPRINTF_ERROR              fprintf(stderr, DBGPRINTF_EXSTR_FORMAT, "ERROR", CUTPATH(__FILE__), __FUNCTION__, __LINE__), __DBGPRINTF
#else
#	define DBGPRINTF_ERROR(...)         ((void)0)
#endif

#if(DBGPRINTF_LEVEL >= DBGPRINTF_LEVEL_FATAL)
#	define DBGPRINTF_FATAL              fprintf(stderr, DBGPRINTF_EXSTR_FORMAT, "FATAL", CUTPATH(__FILE__), __FUNCTION__, __LINE__), __DBGPRINTF
#else
#	define DBGPRINTF_FATAL(...)			((void)0)
#endif

#if(DBGPRINTF_MEMORY_ACTIVE)
#	define DBGPRINTF_MEMORY				fprintf(stderr, DBGPRINTF_EXSTR_FORMAT, "MEMORY", CUTPATH(__FILE__), __FUNCTION__, __LINE__), __DBGPRINTF
#else
#	define DBGPRINTF_MEMORY(...)        ((void)0)
#endif

#if (DBGPRINTF_PERFORMANCE_ACTIVE)
#	define DBGPRINTF_PERFORMANCE        fprintf(stderr, DBGPRINTF_EXSTR_FORMAT, "PERFORMANCE", CUTPATH(__FILE__), __FUNCTION__, __LINE__), __DBGPRINTF
#else
#	define DBGPRINTF_PERFORMANCE(...)   ((void)0)
#endif

#define DBGPRINTF                       DBGPRINTF_ERROR

#else
#	define DBGPRINTF_DEBUG_VERBOSE(...) ((void)0)
#	define DBGPRINTF_DEBUG(...)         ((void)0)
#	define DBGPRINTF_INFO(...)          ((void)0)
#	define DBGPRINTF_WARNING(...)       ((void)0)
#	define DBGPRINTF_ERROR(...)         ((void)0)
#	define DBGPRINTF_FATAL(...)         ((void)0)
#	define DBGPRINTF_MEMORY(...)        ((void)0)
#	define DBGPRINTF_PERFORMANCE(...)   ((void)0)
#	define DBGPRINTF(...)               ((void)0)
#endif

// debugging helpers
#define DEBUG_ONLY_S(arg) (DEBUG_ONLY(arg))
#define RELEASE_ONLY_S(arg) (RELEASE_ONLY(arg))
#define DEBUG_RELEASE_S(dbgarg, relarg) (DEBUG_RELEASE(dbgarg, relarg))
#ifdef EG_DEBUGGER
#	define DEBUG_ONLY(arg) arg
#	define RELEASE_ONLY(arg) ((void)0)
#	define DEBUG_RELEASE(dbgarg, relarg) dbgarg
#else
#	define DEBUG_ONLY(arg) ((void)0)
#	define RELEASE_ONLY(arg) arg
#	define DEBUG_RELEASE(dbgarg, relarg) relarg
#endif

#ifndef _BOOLEAN
// This has to be defined for C to be an unsigned type of the same size as the C++ builtin type bool.
// That size is 1 on all currently supported platforms.
// The C++ wrapper is not compatible to platforms, which define bool to a different size in C99 than in C++
#if !defined __cplusplus && !defined bool // bool is already defined in C99 (which is used in our xcode-projects), but not in the older C version used in VS (which is not conforming to any official standard, but more like C89 + parts of C99)
typedef unsigned char       bool;	// Boolean value type.
#endif
#define _BOOLEAN
#endif

#ifndef _UINT32
typedef unsigned int		uint32;	// Unsigned 32 bit value
#define _UINT32
#endif

#ifndef _UINT16
typedef unsigned short		uint16;	// Unsigned 16 bit value
#define _UINT16
#endif

#ifndef _UINT8
#ifndef _EG_MARMALADE_PLATFORM
typedef unsigned char		uint8;	// Unsigned 8  bit value
#define _UINT8
#endif
#endif

#ifndef _INT32
typedef signed int			int32;	// Signed 32 bit value
#define _INT32
#endif

#ifndef _INT16
typedef signed short		int16;	// Signed 16 bit value
#define _INT16
#endif

#ifndef _INT8
#ifndef _EG_MARMALADE_PLATFORM
typedef signed char			 int8;	// Signed 8  bit value
#define _INT8
#endif
#endif

#define CALLBACK_INIT(pcb,pcf,pcx) {(pcb)->pfnNotify = (PFNNOTIFY)(pcf); (pcb)->pNotifyData = (pcx);}

#endif
