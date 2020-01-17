#pragma once
#include "Engine/HAL/SPlatform.h"

#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatformPakFile.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatformPakFile.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatformPakFile.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatformPakFile.h"
#endif