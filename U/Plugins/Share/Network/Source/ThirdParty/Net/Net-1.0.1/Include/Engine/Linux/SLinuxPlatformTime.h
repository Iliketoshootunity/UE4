#pragma once
#include "Engine/GenericPlatform/SGenericPlatformTime.h"

struct SLinuxPlatformTime : public SGenericPlatformTime
{
public:
	static void SystemTime(int32& Year, int32& Month, int32& DayOfWeek, int32& Day, int32& Hour, int32& Min, int32& Sec, int32& MSec);

    static uint64 GetTickCount();

    static int32 GetTimeOfDay(struct timeval *tv, void *tz);

    static double Seconds();
    
    static uint32 Cycles();

    static uint64 Cycles64();

	static FORCEINLINE void yield(const uint32 ulTime = 10)
	{
		struct timeval sleeptime;
		sleeptime.tv_sec = 0;
		sleeptime.tv_usec = ulTime * 1000;
		select(0, 0, 0, 0, &sleeptime);
	}

	static FORCEINLINE void yieldus(const uint32 ulUSTime = 1)
	{
		struct timeval sleeptime;
		sleeptime.tv_sec = 0;
		sleeptime.tv_usec = ulUSTime;
		select(0, 0, 0, 0, &sleeptime);
	}
    
};

typedef SLinuxPlatformTime  SPlatformTime;