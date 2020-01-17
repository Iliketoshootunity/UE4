#pragma once
#include "Engine/HAL/SPlatform.h"

enum enumClientConnectInfo 
{
	enumClientConnectCreate,
	enumClientConnectClose
};

enum enumServerConnectInfo
{
	enumServerConnectCreate,
	enumServerConnectClose
};

typedef void (*CALLBACK_TCP_CLIENT_EVENT)( void *lpParam, const uint32 ulnEventType );
typedef void (*CALLBACK_TCP_SERVER_EVENT)(void *lpParam, const uint32 ulnID, const uint32 ulnEventType);

class ITcpClient
{
public:
	virtual bool			Startup() = 0;
	virtual int32			Cleanup() = 0;
	virtual int32			Shutdown() = 0;
	virtual int32			Release() = 0;
	virtual bool			ConnectTo( char *strAddr, const uint16 usPort) = 0;
	virtual bool			Disconnect() = 0;
	virtual void			RegisterMsgFilter( void *lpParam, CALLBACK_TCP_CLIENT_EVENT pfnEventNotify ) = 0;
	virtual bool			SendPackToServer(const void* pPack, uint32 pack_len) = 0;
	virtual const void		*GetPackFromServer(uint32&length ) = 0;
	virtual int32			FlushData() = 0;
};

class ITcpServer
{
public:
    virtual int32			Startup() = 0;
    virtual int32			Release() = 0;
    virtual int32			OpenSvc(const uint16 usnPort) = 0;
    virtual int32			CloseService() = 0;
    virtual int32			RegisterMsgFilter(void *lpParam, CALLBACK_TCP_SERVER_EVENT pfnEventNotify) = 0;
    virtual bool			SendPackToClient(const uint32 ClientID, const void* pData) = 0;
    virtual const void		*GetPackFromClient(const uint32 ulnClientID, uint32&datalength) = 0;
    virtual int32			ShutdownClient(const uint32 ulnClientID) = 0;
    virtual const char		*GetClientInfo(const uint32 ulnClientID) = 0;
};

void CreateTcpClient(ITcpClient *&pClient, int32 max_send = 64 * 1024, int32 max_receive = 512 * 1024);
void CreateTcpServer(ITcpServer *&pServer, uint32 number, uint32 max_send = 256*1024, uint32 max_receive = 128*1024 );

///////////////////////////////////////////////////////////////////////

enum enumUdpClientConnectInfo
{
	enumUdpClientConnectCreate,
	enumUdpClientConnectClose
};

enum enumUdpServerConnectInfo
{
	enumUdpServerConnectCreate,
	enumUdpServerConnectClose
};

typedef void(*CALLBACK_UDP_CLIENT_EVENT)(void *lpParam, const uint32 ulnEventType);
typedef void(*CALLBACK_UDP_SERVER_EVENT)(void *lpParam, const uint32 ulnID, const uint32 ulnEventType);

class IUdpClient
{
public:
	virtual void			Initialize(int32 max_send = 1024 * 1024, int32 max_recv = 1024 * 1024) = 0;
	virtual int32			Startup() = 0;
	virtual int32			Cleanup() = 0;
	virtual int32			Shutdown() = 0;
	virtual int32			Release() = 0;
	virtual int32			ConnectTo(char *strAddr, const uint16 usPort) = 0;
	virtual int32			Disconnect() = 0;
	virtual void			RegisterMsgFilter(void *lpParam, CALLBACK_UDP_CLIENT_EVENT pfnEventNotify) = 0;

	//包头里已经有了包长度
	virtual int32			SendPackToServer(const void *pData) = 0;
	virtual const void		*GetPackFromServer(uint32 &length) = 0;

	virtual int32			FlushData() = 0;
};

class IUdpServer
{
public:
    virtual int32			Startup() = 0;
    virtual int32			Release() = 0;
    virtual int32			OpenSvc(const uint16 usnPort) = 0;
    virtual int32			CloseService() = 0;
    virtual int32			RegisterMsgFilter(void *lpParam, CALLBACK_UDP_SERVER_EVENT pfnEventNotify) = 0;
    virtual int32			SendPackToClient(const uint32 ulnClientID, const void *pData) = 0;
    virtual const void		*GetPackFromClient(const uint32 ulnClientID, uint32 &datalength) = 0;
    virtual int32			ShutdownClient(const uint32 ulnClientID) = 0;
    virtual const char		*GetClientInfo(const uint32 ulnClientID) = 0;
};

void CreateUdpServer(IUdpServer *&pServer, uint32 number, uint32 max_send = 256 * 1024, uint32 max_receive = 32 * 1024);
void CreateUdpClient(IUdpClient *&pClient, int32 max_send = 1024 * 1024, int32 max_receive = 1024 * 1024);

