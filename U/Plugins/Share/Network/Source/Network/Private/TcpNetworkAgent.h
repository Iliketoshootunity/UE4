// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ITcpNetworkAgent.h"
/**
 * 
 */
class IMsgProtocol;

class NETWORK_API FTcpNetworkAgent : public ITcpNetworkAgent
{
	
public:
	FTcpNetworkAgent();
	virtual ~FTcpNetworkAgent();

public:
	static void	TcpClientCallBack(void* lpParam, const uint32 ulnEventType);

public:
    virtual bool	    Initialize(IMsgProtocol* pMsgProtocol) override;
    virtual void        Shutdown() override;

	virtual void		Tick(float fDeltaTime) override;

	///////////////////////////Client//////////////////////

    virtual bool		Startup() override;
    virtual void		Close() override;
    virtual bool		ConnectToGameSvr(const char* pIpAddress, unsigned short uPort) override;
    virtual void		DisconnectGameSvr() override;
    virtual bool		SendDataToServer(const char* pBuffer, int32 length) override;
	virtual bool		SendMsgImmediately(const char* pBuffer, int32 length) override;
    virtual bool		IsConnecting() override;
	virtual uint32		GetHeadSize() override;
	int32				PutPackHead(char* buf, int32 protocolID, int32 Length, int16 mark) override;

public:
	inline void			SetGameServConnectState(bool bConnect)
	{
		m_bTcpIsGameServConnecting = bConnect;
	}

private:
	void				ProcessGameServerData(const char *pBuffer, unsigned int nSize);
	
private:
	IMsgProtocol *		m_pMsgProtocol;

	ITcpClient*			m_pTcpGameSvrClient;
	bool				m_bTcpIsGameServConnecting;
};