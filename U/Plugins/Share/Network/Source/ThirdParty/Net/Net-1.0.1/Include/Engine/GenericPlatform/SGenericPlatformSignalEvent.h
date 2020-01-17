#pragma once
#include "Engine/HAL/SPlatform.h"

struct SGenericPlatformSignalEvent
{
public:
    virtual bool CreateEvent() = 0;
    //dwMilliseconds ºÁÃë
    virtual void WaitForSingleObject(uint32 dwMilliseconds) = 0;
    virtual int32 SetEvent() = 0;
    virtual void CloseEvent() = 0;
    virtual long TimevalDiff(timeval & now, timeval & last)
    {
        long timediff;

        if ((timediff = (now.tv_sec - last.tv_sec)) > 5)
            timediff = 5;

        return timediff * 1000000 + now.tv_usec - last.tv_usec;
    }

	virtual bool IsValid()
	{
		return bValid;
	}

private:
	friend struct SAndroidPlatformSignalEvent;
	friend struct SIosPlatformSignalEvent;
	friend struct SLinuxPlatformSignalEvent;
	friend struct SWindowsPlatformSignalEvent;
	bool                        bValid;
};