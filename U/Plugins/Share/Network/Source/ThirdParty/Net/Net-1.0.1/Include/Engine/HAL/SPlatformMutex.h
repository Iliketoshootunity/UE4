#pragma once
#include "Engine/HAL/SPlatform.h"

#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatformMutex.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatformMutex.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatformMutex.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatformMutex.h"
#endif