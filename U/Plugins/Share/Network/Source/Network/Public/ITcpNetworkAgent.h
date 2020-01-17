#pragma once
#include "CoreMinimal.h"
#include "NetworkInterface.h"

#ifdef TICK_DELAY_TIME
#undef TICK_DELAY_TIME
#endif
#define TICK_DELAY_TIME (1000/50)


class IMsgProtocol;

/*******************************������************************************
* Desc 	: Client Net Connect Agent Interface
* Author	: WangPeng
* Time		: 2019.6.13
************************************************************************/

class NETWORK_API ITcpNetworkAgent
{
public:
	virtual bool				Initialize(IMsgProtocol* pMsgProtocol) =0;		//	��ʼ����Ϣ�ַ����
    virtual void                Shutdown() = 0;									//	�ͻ����˳�ʱ����
	virtual void				Tick(float fDeltaTime) = 0;						//	Net����

	virtual bool				Startup() = 0;								// Tcp Net ����
	virtual void				Close() =0;									// Tcp Net �ر�
	virtual bool				ConnectToGameSvr(const char* pIpAddress, unsigned short uPort)=0;
	virtual void				DisconnectGameSvr()=0;

	/************************************************************************/
	/*				������Ϣ�������������̴߳�����Ϣ����Ӱ�����߳�Ч��				*/
	/************************************************************************/
	virtual bool				SendDataToServer( const char* pBuffer, int32 length)=0;

	/************************************************************************/
	/*				����������Ϣ����������������Ϣʹ�÷���Ӱ��Ч��					*/
	/************************************************************************/
	virtual bool				SendMsgImmediately(const char* pBuffer, int32 length) = 0;
	virtual bool				IsConnecting()=0;							// Tcp Net �Ƿ����ӳɹ����ɹ���true��ʧ�ܣ�false
	virtual int32				PutPackHead(char* buf, int32 protocolID, int32 Length, int16 mark) = 0;
	virtual uint32				GetHeadSize() = 0;							//��ȡ��ͷ����
};

extern NETWORK_API ITcpNetworkAgent* GetTcpNetworkAgent();				// ��ȡǰ���������Ӵ���