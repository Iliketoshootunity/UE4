#pragma once
#include "Engine/HAL/SPlatform.h"

enum SEventType
{
    EVENT_EPOLLIN = 0,              //�������¼�����ʾ��Ӧ���ļ����������пɶ����ݡ�(�����Զ�SOCKET�����ر�)��
    EVENT_EPOLLHUP,                 //��ʾ��Ӧ���ļ����������Ҷϣ�
    EVENT_EPOLLERR,                 //��ʾ��Ӧ���ļ���������������
    EVENT_EPOLLOUT,                 //�������¼�����ʾ��Ӧ���ļ��������Ͽ���д���ݣ�
    EVENT_EPOLLPRI,                 //��ʾ��Ӧ���ļ��������н��������ݿɶ�������Ӧ�ñ�ʾ�д������ݵ�������
    EVENT_EPOLLET,                  //��EPOLL��Ϊ��Ե����(Edge Triggered)ģʽ�����������ˮƽ����(Level Triggered)��˵�ġ�
    EVENT_EPOLLONESHOT,             //ֻ����һ���¼���������������¼�֮���������Ҫ�����������socket�Ļ�����Ҫ�ٴΰ����socket���뵽EPOLL�����
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
