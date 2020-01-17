#pragma once
#include "Engine/GenericPlatform/SGenericPlatformThread.h"


struct SWindowsPlatformThread : public SGenericPlatformThread
{
private:
	HANDLE          thread_id;

public:
    SWindowsPlatformThread() { status = new_created; }
	virtual ~SWindowsPlatformThread()
	{
		if (status == running) {
			TerminateThread(thread_id, 0);
		}
	}

	static unsigned long WINAPI ThreadProc(void* lpParameter)
	{
		SGenericPlatformThread* thread = (SGenericPlatformThread*)lpParameter;
		thread->action();
		return 0;
	}

	FORCEINLINE bool start()
	{
		if (status == new_created) {
			thread_id = CreateThread(NULL, 0, &SWindowsPlatformThread::ThreadProc, this, 0, NULL);
			if (thread_id) {
				status = running;
				return true;
			}
		}
		return false;
	}

	FORCEINLINE bool stop()
	{
		if (status == running) {
			TerminateThread(thread_id, 0);
			status = stopped;
			return true;
		}

		return false;
	}

	virtual void* action() = 0;

	FORCEINLINE bool isStop()
	{
		return status != running;
	}

	FORCEINLINE void yield(const uint32 ulTime = 10)
	{
		Sleep(ulTime);
	}

	FORCEINLINE void yieldus(const uint32 ulUSTime = 1)
	{
		Sleep(ulUSTime/1000);
	}

	FORCEINLINE long TimevalDiff(timeval &now, timeval &past)
	{
		long timediff = 0;

		if ((timediff = (now.tv_sec - past.tv_sec)) > 5)
			timediff = 5;

		return timediff * 1000000 + now.tv_usec - past.tv_usec;
	}

	FORCEINLINE void WaitNextFrame(uint32 ulUSTime = TICK_DELAY_TIME)
	{
		yieldus(TICK_DELAY_TIME);
	}

};

typedef SWindowsPlatformThread SThread;