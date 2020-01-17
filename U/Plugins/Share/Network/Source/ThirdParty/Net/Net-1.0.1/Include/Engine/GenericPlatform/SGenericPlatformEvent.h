#pragma once
#include "Engine/HAL/SPlatform.h"

enum SEventType
{
    EVENT_EPOLLIN = 0,              //触发该事件，表示对应的文件描述符上有可读数据。(包括对端SOCKET正常关闭)；
    EVENT_EPOLLHUP,                 //表示对应的文件描述符被挂断；
    EVENT_EPOLLERR,                 //表示对应的文件描述符发生错误；
    EVENT_EPOLLOUT,                 //触发该事件，表示对应的文件描述符上可以写数据；
    EVENT_EPOLLPRI,                 //表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
    EVENT_EPOLLET,                  //将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
    EVENT_EPOLLONESHOT,             //只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里。
};

typedef void (*SEventFunc)(void* pThis, int32 fd, SEventType type);


typedef struct tagEventData
{
public:
	tagEventData()
	{
		fd = -1;
		pThis = nullptr;
	}
	~tagEventData()
	{
		fd = -1;
		pThis = nullptr;
	}

	void Reset()
	{
		fd = -1;
		pThis = nullptr;
	}

	bool	IsValid()
	{
		return (fd != -1 && pThis != nullptr);
	}
public:
    int32       fd;
    uint32      Event;
	void*		pThis;
	SEventFunc	CallBack;
} SEventData;

struct SGenericPlatformEvent
{
public:
    virtual int32 CreateEvent(int32 nMaxEvent) = 0;
    virtual int32 AddEvent(int32 fd, void* pThis, SEventFunc func) = 0;
    virtual int32 DelEvent(int32 fd) = 0;
    virtual int32 WaitEvent() = 0;
    virtual int32 CloseEvent() = 0;
    virtual bool  IsValid() = 0;
};
