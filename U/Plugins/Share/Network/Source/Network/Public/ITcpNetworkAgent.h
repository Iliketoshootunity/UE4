#pragma once
#include "CoreMinimal.h"
#include "NetworkInterface.h"

#ifdef TICK_DELAY_TIME
#undef TICK_DELAY_TIME
#endif
#define TICK_DELAY_TIME (1000/50)


class IMsgProtocol;

/*******************************类声明************************************
* Desc 	: Client Net Connect Agent Interface
* Author	: WangPeng
* Time		: 2019.6.13
************************************************************************/

class NETWORK_API ITcpNetworkAgent
{
public:
	virtual bool				Initialize(IMsgProtocol* pMsgProtocol) =0;		//	初始化消息分发句柄
    virtual void                Shutdown() = 0;									//	客户端退出时调用
	virtual void				Tick(float fDeltaTime) = 0;						//	Net心跳

	virtual bool				Startup() = 0;								// Tcp Net 启动
	virtual void				Close() =0;									// Tcp Net 关闭
	virtual bool				ConnectToGameSvr(const char* pIpAddress, unsigned short uPort)=0;
	virtual void				DisconnectGameSvr()=0;

	/************************************************************************/
	/*				发送消息到服务器，多线程处理消息，不影响主线程效率				*/
	/************************************************************************/
	virtual bool				SendDataToServer( const char* pBuffer, int32 length)=0;

	/************************************************************************/
	/*				立即发送消息到服务器，特殊消息使用否则影响效率					*/
	/************************************************************************/
	virtual bool				SendMsgImmediately(const char* pBuffer, int32 length) = 0;
	virtual bool				IsConnecting()=0;							// Tcp Net 是否连接成功，成功：true；失败：false
	virtual int32				PutPackHead(char* buf, int32 protocolID, int32 Length, int16 mark) = 0;
	virtual uint32				GetHeadSize() = 0;							//获取包头长度
};

extern NETWORK_API ITcpNetworkAgent* GetTcpNetworkAgent();				// 获取前段网络连接代理