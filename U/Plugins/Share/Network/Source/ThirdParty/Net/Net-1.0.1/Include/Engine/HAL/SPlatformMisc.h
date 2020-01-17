#pragma once
#include "Engine/HAL/SPlatform.h"

#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatformMisc.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatformMisc.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatformMisc.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatformMisc.h"
#endif

#if UE_BUILD_DEBUG
#define EG_ASSERT(x)	if (!(x)) SPlatformMisc::EG_AssertFailed((char*)__FILE__, __LINE__)
#else
#define EG_ASSERT(x)	NULL
#endif