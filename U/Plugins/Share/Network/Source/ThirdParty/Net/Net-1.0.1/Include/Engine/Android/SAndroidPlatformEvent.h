#pragma once
#include "Engine/GenericPlatform/SGenericPlatformEvent.h"

struct SAndroidPlatformEvent : public SGenericPlatformEvent
{
private:
    int32						nEventFD;
	SEventData* m_pEvent;
	int32									m_nMaxEvent;
public:
    SAndroidPlatformEvent();
    virtual ~SAndroidPlatformEvent();
    virtual int32 CreateEvent(int32 nMaxEvent) override;
    virtual int32 AddEvent(int32 fd, void* pThis, SEventFunc func) override;
    virtual int32 DelEvent(int32 fd) override;
    virtual int32 WaitEvent() override;
    virtual int32 CloseEvent() override;
    virtual bool  IsValid() override;

private:
	int32		  GetEventFreeIndex();
	int32		  FindEventIndex(int32 fd);
};

typedef SAndroidPlatformEvent SEvent;