#pragma once
#include "Engine/HAL/SPlatform.h"

#if PLATFORM_WINDOWS
#include "Engine/Windows/SWindowsPlatformTcpSocket.h"
#elif PLATFORM_LINUX
#include "Engine/Linux/SLinuxPlatformTcpSocket.h"
#elif PLATFORM_ANDROID
#include "Engine/Android/SAndroidPlatformTcpSocket.h"
#elif PLATFORM_IOS || PLATFORM_MAC
#include "Engine/Ios/SIosPlatformTcpSocket.h"
#endif
