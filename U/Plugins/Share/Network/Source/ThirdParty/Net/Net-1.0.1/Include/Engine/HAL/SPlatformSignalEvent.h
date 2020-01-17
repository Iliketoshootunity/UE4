#pragma once
#include "Engine/HAL/SPlatform.h"

#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatformSignalEvent.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatformSignalEvent.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatformSignalEvent.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatformSignalEvent.h"
#endif