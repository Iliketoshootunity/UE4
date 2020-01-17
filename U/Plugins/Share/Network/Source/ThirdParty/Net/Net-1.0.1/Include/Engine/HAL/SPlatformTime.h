#pragma once
#include "Engine/HAL/SPlatform.h"

#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatformTime.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatformTime.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatformTime.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatformTime.h"
#endif