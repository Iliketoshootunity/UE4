#pragma once
#include "Engine/HAL/SPlatform.h"

#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatformFile.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatformFile.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatformFile.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatformFile.h"
#endif