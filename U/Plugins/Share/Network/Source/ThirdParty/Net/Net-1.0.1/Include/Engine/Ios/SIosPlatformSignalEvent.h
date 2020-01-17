#pragma once
#include "Engine/GenericPlatform/SGenericPlatformSignalEvent.h"

struct SIosPlatformSignalEvent : public  SGenericPlatformSignalEvent
{
private:
    int32                       PipeEvent[2];
public:
    SIosPlatformSignalEvent();
	virtual ~SIosPlatformSignalEvent();
public:
    virtual bool CreateEvent() override;
    virtual void WaitForSingleObject(uint32 dwMilliseconds) override;
    virtual int32 SetEvent() override;
    virtual void CloseEvent() override;
};

typedef SIosPlatformSignalEvent  SSignalEvent;
