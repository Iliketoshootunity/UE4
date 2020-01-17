#pragma once
#include "Engine/GenericPlatform/SGenericPlatformMutex.h"


struct SAndroidPlatformMutex : public SGenericPlatformMutex
{
private:
    pthread_mutex_t mutex;

public:
    /*
    *使用自旋锁
    */
    SAndroidPlatformMutex()
    {
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutex_init(&mutex, &attr);
    }

    /*  使用互斥锁  mutex_type
    * PTHREAD_MUTEX_TIMED_NP = 0，这是缺省值，也就是普通锁。当一个线程加锁以后，其余请求锁的线程将形成一个等待队列，并在解锁后按优先级获得锁。这种锁策略保证了资源分配的公平性。
    * PTHREAD_MUTEX_RECURSIVE_NP = 1，嵌套锁，允许同一个线程对同一个锁成功获得多次，并通过多次unlock解锁。如果是不同线程请求，则在加锁线程解锁时重新竞争。
    * PTHREAD_MUTEX_ERRORCHECK_NP = 2，检错锁，如果同一个线程请求同一个锁，则返回EDEADLK，否则与PTHREAD_MUTEX_TIMED_NP类型动作相同。这样就保证当不允许多次加锁时不会出现最简单情况下的死锁。
    * PTHREAD_MUTEX_ADAPTIVE_NP = 3，适应锁，动作最简单的锁类型，仅等待解锁后重新竞争。
    */
    SAndroidPlatformMutex(int mutex_type)
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        if (pthread_mutexattr_settype(&attr, mutex_type))
        {
            // Invalid mutex_type, use default type;
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
        }
        pthread_mutex_init(&mutex, &attr);
    }

    ~SAndroidPlatformMutex()
    {
		pthread_mutex_destroy(&mutex);
    }

    virtual bool trylock() override
    {
        return pthread_mutex_trylock(&mutex) == 0 ? true : false;
    }

    virtual void lock() override
    {
		pthread_mutex_lock(&mutex);
        
    }

    virtual void unlock() override
    {
		pthread_mutex_unlock(&mutex);  
    }
};

typedef SAndroidPlatformMutex   SMutex;