#pragma once
#include "Engine/GenericPlatform/SGenericPlatformTime.h"

struct SWindowsPlatformTime : public SGenericPlatformTime
{
	static void SystemTime(int32& Year, int32& Month, int32& DayOfWeek, int32& Day, int32& Hour, int32& Min, int32& Sec, int32& MSec);

    static uint64 GetTickCount();

    static int32 GetTimeOfDay(struct timeval *tv, void *tz);

    static FORCEINLINE double Seconds()
    {
        struct timeval tv;
        GetTimeOfDay(&tv, NULL);
        return ((double)tv.tv_sec) + (((double)tv.tv_usec) / 1000000.0);
    }

    static FORCEINLINE uint32 Cycles()
    {
        struct timeval tv;
        GetTimeOfDay(&tv, NULL);
        return (uint32)((((uint64)tv.tv_sec) * 1000000ULL) + (((uint64)tv.tv_usec)));
    }

    static FORCEINLINE uint64 Cycles64()
    {
        struct timeval tv;
        GetTimeOfDay(&tv, NULL);
        return ((((uint64)tv.tv_sec) * 1000000ULL) + (((uint64)tv.tv_usec)));
    }

	static FORCEINLINE void yield(const uint32 ulTime = 10)
	{
		Sleep(ulTime);
	}

	static FORCEINLINE void yieldus(const uint32 ulUSTime = 1)
	{
		Sleep(ulUSTime / 1000);
	}
};

typedef SWindowsPlatformTime  SPlatformTime;