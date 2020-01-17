#pragma once
#include "Engine/HAL/SPlatform.h"

#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatformUdpSocket.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatformUdpSocket.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatformUdpSocket.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatformUdpSocket.h"
#endif