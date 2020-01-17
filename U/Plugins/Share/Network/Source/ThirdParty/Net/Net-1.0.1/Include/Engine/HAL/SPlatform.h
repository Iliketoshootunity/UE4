#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif // !_WINSOCK_DEPRECATED_NO_WARNINGS


#define GUID_MAX_LEN 64

#ifndef NULL
#define NULL			( 0 )
#endif

#ifndef INVALID
#define INVALID			(-1)
#endif

#ifndef INVALID_VALUE
#define INVALID_VALUE	( -1 )
#define SUCCESS_VALUE	( 0 )
#endif


#ifndef UE_BUILD_DEBUG
#if _DEBUG
#define  UE_BUILD_DEBUG				1
#else
#define  UE_BUILD_DEBUG				0
#endif
#endif


#if !defined(PLATFORM_WINDOWS)
    #if (defined(_WIN32) || defined(_WIN64)) && !defined(PLATFORM_IOS)
        #define PLATFORM_WINDOWS 1
    #else
        #define PLATFORM_WINDOWS 0
    #endif 
#endif

#if !defined(PLATFORM_LINUX)
    #if ((defined(__linux) || defined(__linux__)) && !PLATFORM_ANDROID)
        #define PLATFORM_LINUX 1
    #else
        #define PLATFORM_LINUX 0
    #endif 
#endif

#if !defined(PLATFORM_ANDROID)
    #if defined(__ANDROID__) || defined(ANDROID)
        #define PLATFORM_ANDROID 1
    #else
        #define PLATFORM_ANDROID 0
    #endif 
#endif

#if !defined(PLATFORM_IOS)
    #if defined(__APPLE__)
        #include <TargetConditionals.h>
        #if defined(TARGET_OS_IPHONE)
            #define PLATFORM_IOS 1
        #else
            #define PLATFORM_IOS 0
        #endif
    #else
        #define PLATFORM_IOS 0
    #endif
#endif

#if !defined(PLATFORM_MAC)
    #if defined(__APPLE__)
        #include "TargetConditionals.h"
        #if defined(TARGET_OS_MAC)
            #define PLATFORM_MAC 1
        #else
            #define PLATFORM_MAC 0
        #endif
    #else
        #define PLATFORM_MAC 0
    #endif
#endif

#if PLATFORM_WINDOWS //|| PLATFORM_LINUX
#pragma warning(disable:4996)
#endif




#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatform.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatform.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatform.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatform.h"
#endif

#if !WITH_UNREAL4

//~ Unsigned base types.
/// An 8-bit unsigned integer.
typedef SPlatformTypes::uint8		uint8;
/// A 16-bit unsigned integer.
typedef SPlatformTypes::uint16		uint16;
/// A 32-bit unsigned integer.
typedef SPlatformTypes::uint32		uint32;
/// A 64-bit unsigned integer.
typedef SPlatformTypes::uint64		uint64;

//~ Signed base types.
/// An 8-bit signed integer.
typedef	SPlatformTypes::int8		int8;
/// A 16-bit signed integer.
typedef SPlatformTypes::int16		int16;
/// A 32-bit signed integer.
typedef SPlatformTypes::int32		int32;
/// A 64-bit signed integer.
typedef SPlatformTypes::int64		int64;

//~ Character types.
/// An ANSI character. Normally a signed type.
typedef SPlatformTypes::ANSICHAR	ANSICHAR;
/// A wide character. Normally a signed type.
typedef SPlatformTypes::WIDECHAR	WIDECHAR;
/// Either ANSICHAR or WIDECHAR, depending on whether the platform supports wide characters or the requirements of the licensee.
typedef SPlatformTypes::TCHAR		SCHAR;
/// An 8-bit character containing a UTF8 (Unicode, 8-bit, variable-width) code unit.
typedef SPlatformTypes::CHAR8		UTF8CHAR;
/// A 16-bit character containing a UCS2 (Unicode, 16-bit, fixed-width) code unit, used for compatibility with 'Windows TCHAR' across multiple platforms.
typedef SPlatformTypes::CHAR16		UCS2CHAR;
/// A 16-bit character containing a UTF16 (Unicode, 16-bit, variable-width) code unit.
typedef SPlatformTypes::CHAR16		UTF16CHAR;
/// A 32-bit character containing a UTF32 (Unicode, 32-bit, fixed-width) code unit.
typedef SPlatformTypes::CHAR32		UTF32CHAR;

/// An unsigned integer the same size as a pointer
typedef SPlatformTypes::UPTRINT UPTRINT;
/// A signed integer the same size as a pointer
typedef SPlatformTypes::PTRINT PTRINT;
/// An unsigned integer the same size as a pointer, the same as UPTRINT
typedef SPlatformTypes::SIZE_T SIZE_T;
/// An integer the same size as a pointer, the same as PTRINT
typedef SPlatformTypes::SSIZE_T SSIZE_T;

/// The type of the NULL constant.
typedef SPlatformTypes::TYPE_OF_NULL	TYPE_OF_NULL;
/// The type of the C++ nullptr keyword.
typedef SPlatformTypes::TYPE_OF_NULLPTR	TYPE_OF_NULLPTR;

#else
#include "CoreMinimal.h"
#endif

typedef struct { volatile unsigned long counter; } atomic_type;

#if PLATFORM_WINDOWS
typedef void*						MUTEX_HANDLE;
#elif PLATFORM_LINUX
typedef pthread_mutex_t				MUTEX_HANDLE;
#elif PLATFORM_ANDROID
typedef pthread_mutex_t				MUTEX_HANDLE;
#elif PLATFORM_IOS || PLATFORM_MAC
typedef pthread_mutex_t				MUTEX_HANDLE;
#endif

#ifndef TICK_DELAY_TIME
#define TICK_DELAY_TIME (1000/50)		//ms
#endif

#ifndef FORCEINLINE
#if PLATFORM_WINDOWS
#define FORCEINLINE __forceinline
#else
#if UE_BUILD_DEBUG
#define FORCEINLINE inline 
#else
#define FORCEINLINE inline __attribute__ ((always_inline))	
#endif
#endif
#endif

#if !PLATFORM_LINUX
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL	0
#endif
#endif

#ifndef MAX_EP
#define MAX_EP 500
#endif

#ifndef MAX_EP_WAIT
#define MAX_EP_WAIT (MAX_EP/100)
#endif

#if PLATFORM_WINDOWS
#ifndef socklen_t
#define socklen_t	int32
#endif
#else
#ifndef SOCKET
#define SOCKET int32
#endif
#endif

typedef socklen_t SOCKLEN;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif 

#ifndef SOCKET_ERROR
#define SOCKET_ERROR    -1
#endif

#ifndef FILE_CURRENT
#define FILE_CURRENT	1
#endif

#ifndef FILE_END
#define FILE_END		2
#endif

#ifndef FILE_BEGIN
#define FILE_BEGIN		0
#endif




