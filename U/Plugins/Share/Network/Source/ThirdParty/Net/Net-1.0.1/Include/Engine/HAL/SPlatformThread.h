#pragma once
#include "Engine/HAL/SPlatform.h"

#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatformThread.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatformThread.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatformThread.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatformThread.h"
#endif