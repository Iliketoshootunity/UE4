#pragma once
#include "Engine/GenericPlatform/SGenericPlatformSignalEvent.h"

struct SWindowsPlatformSignalEvent : public  SGenericPlatformSignalEvent
{
private:
    HANDLE                      PipeEvent;
public:
    SWindowsPlatformSignalEvent();
	virtual  ~SWindowsPlatformSignalEvent();
public:
    virtual bool CreateEvent() override;
    //参数必须大于10ms
    virtual void WaitForSingleObject(uint32 dwMilliseconds) override;
    virtual int32 SetEvent() override;
    virtual void CloseEvent() override;
};

typedef SWindowsPlatformSignalEvent  SSignalEvent;