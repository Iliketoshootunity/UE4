#pragma once
#include "Engine/GenericPlatform/SGenericPlatformSignalEvent.h"

struct SLinuxPlatformSignalEvent : public  SGenericPlatformSignalEvent
{
private:
    int32                       PipeEvent[2];
public:
    SLinuxPlatformSignalEvent();
	virtual ~SLinuxPlatformSignalEvent();
public:
    virtual bool CreateEvent() override;
    virtual void WaitForSingleObject(uint32 dwMilliseconds) override;
    virtual int32 SetEvent() override;
    virtual void CloseEvent() override;
};

typedef SLinuxPlatformSignalEvent  SSignalEvent;
