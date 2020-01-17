#pragma once
#include "Engine/HAL/SPlatform.h"

#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatformPath.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatformPath.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatformPath.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatformPath.h"
#endif
