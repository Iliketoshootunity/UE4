#pragma once

#include "Engine/GenericPlatform/SGenericPlatformThread.h"

struct SLinuxPlatformThread : public SGenericPlatformThread
{
private:
	pthread_t       thread_id;
	pthread_attr_t  th_attr;
	int32			pipeCancel[2];
public:
    SLinuxPlatformThread()
	{ 
		status = new_created; 
		pthread_attr_init(&th_attr);
		pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_DETACHED);
	}

	virtual ~SLinuxPlatformThread()
	{
		
	}

	static void* ThreadProc(void* lpParameter)
	{
		SGenericPlatformThread* thread = (SGenericPlatformThread*)lpParameter;
		thread->action();
		return NULL;
	}

	FORCEINLINE bool start()
	{
		pipe(pipeCancel);

		if (status == new_created
			&& !pthread_create(&thread_id, &th_attr, &SLinuxPlatformThread::ThreadProc, this)) {
			status = running;
			return true;
		}
		return false;
	}

	FORCEINLINE bool stop()
	{
		if (status == running) 
		{
			int bignum = 1;
			write(pipeCancel[1], &bignum, sizeof(bignum));
			status = stopped;
			return true;
		}

		return false;
	}

	virtual void* action() = 0;

	FORCEINLINE bool isStop()
	{
		static fd_set read_fds;
		struct timeval timeout = { 0,0 };
		FD_ZERO(&read_fds);
		FD_SET(pipeCancel[0], &read_fds);

		int result = select(pipeCancel[0] + 1, &read_fds, NULL, NULL, &timeout);
		if (result > 0 && FD_ISSET(pipeCancel[0], &read_fds))
		{
			return true;
		}
		return false;
	}

	FORCEINLINE void yield(const uint32 ulTime = 10)
	{
		struct timeval sleeptime;
		sleeptime.tv_sec = 0;
		sleeptime.tv_usec = ulTime * 1000;
		select(0, 0, 0, 0, &sleeptime);
	}

	FORCEINLINE void yieldus(const uint32 ulUSTime = 1)
	{
		struct timeval sleeptime;
		sleeptime.tv_sec = 0;
		sleeptime.tv_usec = ulUSTime;
		select(0, 0, 0, 0, &sleeptime);
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
		static timeval lastframe = { 0,0 };
		timeval now;
		timeval time_remain = { 0,0 };
		long frame_interval = ulUSTime * 1000; // in us
		long time_remain_us;

		gettimeofday(&now, NULL);

		if ((time_remain_us = frame_interval - TimevalDiff(now, lastframe)) > 0)
		{
			time_remain.tv_usec = time_remain_us;
			//nanosleep( &time_remain, NULL);
			select(1, NULL, NULL, NULL, &time_remain);
		}
		gettimeofday(&lastframe, NULL);
	}
};


typedef SLinuxPlatformThread SThread;