#pragma once

#if WITH_UNREAL4

#include "Windows/AllowWindowsPlatformTypes.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include <sal.h>
#include <time.h> 
#include <direct.h>
#include "conio.h"
#include <malloc.h>
#include "Windows/HideWindowsPlatformTypes.h"

#else

#ifndef WIN32_LEAN_AND_MEAN 
#include <winsock2.h>
#include <Ws2tcpip.h>
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <sal.h>
#include <time.h> 
#include <direct.h>
#include "conio.h"
#include <malloc.h>
#endif


#include "Engine/GenericPlatform/SGenericPlatform.h"

#if !WITH_UNREAL4
/**
* Windows specific types
**/
struct SWindowsPlatformTypes : public SGenericPlatformTypes
{
#ifdef _WIN64
	typedef unsigned __int64	SIZE_T;
	typedef __int64				SSIZE_T;
#else
	typedef unsigned long		SIZE_T;
	typedef long				SSIZE_T;
#endif
};

typedef SWindowsPlatformTypes SPlatformTypes;

#endif

