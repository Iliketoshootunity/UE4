#pragma once
#include "Engine/GenericPlatform/SGenericPlatformMutex.h"


struct SAndroidPlatformMutex : public SGenericPlatformMutex
{
private:
    pthread_mutex_t mutex;

public:
    /*
    *ʹ��������
    */
    SAndroidPlatformMutex()
    {
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutex_init(&mutex, &attr);
    }

    /*  ʹ�û�����  mutex_type
    * PTHREAD_MUTEX_TIMED_NP = 0������ȱʡֵ��Ҳ������ͨ������һ���̼߳����Ժ��������������߳̽��γ�һ���ȴ����У����ڽ��������ȼ�����������������Ա�֤����Դ����Ĺ�ƽ�ԡ�
    * PTHREAD_MUTEX_RECURSIVE_NP = 1��Ƕ����������ͬһ���̶߳�ͬһ�����ɹ���ö�Σ���ͨ�����unlock����������ǲ�ͬ�߳��������ڼ����߳̽���ʱ���¾�����
    * PTHREAD_MUTEX_ERRORCHECK_NP = 2������������ͬһ���߳�����ͬһ�������򷵻�EDEADLK��������PTHREAD_MUTEX_TIMED_NP���Ͷ�����ͬ�������ͱ�֤���������μ���ʱ��������������µ�������
    * PTHREAD_MUTEX_ADAPTIVE_NP = 3����Ӧ����������򵥵������ͣ����ȴ����������¾�����
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