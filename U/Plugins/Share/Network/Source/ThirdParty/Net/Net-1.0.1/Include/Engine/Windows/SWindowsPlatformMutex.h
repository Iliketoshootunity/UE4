#pragma once

#include "Engine/GenericPlatform/SGenericPlatformMutex.h"

#ifndef SINGLE_PROCESS
#define SINGLE_PROCESS 1        //控制Windows下，单进程和多进程的线程锁机制
#endif // !SINGLE_PROCESS

struct SWindowsPlatformMutex : public SGenericPlatformMutex
{

private:
#if SINGLE_PROCESS
    CRITICAL_SECTION m_CriticalSection;//用于单进程的线程同步
#else
    HANDLE          mutex;   //用于多进程的线程同步
#endif

public:
    SWindowsPlatformMutex()
	{
#if SINGLE_PROCESS
		InitializeCriticalSection(&m_CriticalSection);
#else
		mutex = CreateMutex(NULL, false, NULL);
#endif
	}

    SWindowsPlatformMutex(int mutex_type)
    {
#if SINGLE_PROCESS
        InitializeCriticalSection(&m_CriticalSection);
#else
        mutex = CreateMutex(NULL, false, NULL);
#endif
    }

	virtual ~SWindowsPlatformMutex()
    {
#if SINGLE_PROCESS
        DeleteCriticalSection(&m_CriticalSection);
#else
        CloseHandle(mutex);
#endif
    }

    virtual bool trylock() override
    {
        return false;
    }

    virtual void lock() override
    {
#if SINGLE_PROCESS
        EnterCriticalSection(&m_CriticalSection);
#else
        WaitForSingleObject(mutex, INFINITE);
#endif
    }

    virtual void unlock() override
    {
#if SINGLE_PROCESS
        LeaveCriticalSection(&m_CriticalSection);
#else
        ReleaseMutex(mutex);
#endif
    }
};

typedef SWindowsPlatformMutex   SMutex;
