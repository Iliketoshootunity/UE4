#pragma once
#include "Engine/HAL/SPlatform.h"

#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatformEvent.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatformEvent.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatformEvent.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatformEvent.h"
#endif