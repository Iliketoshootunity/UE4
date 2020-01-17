// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpNetworkAgent.h"
#include "NetworkLog.h"
#include "IMsgProtocol.h"
#include "Netlog.h"
#include "Networkdef.h"
#include "SystemEndian.h"


ITcpNetworkAgent* GetTcpNetworkAgent()
{
	return new FTcpNetworkAgent();
}


FTcpNetworkAgent::FTcpNetworkAgent()
{
	m_pTcpGameSvrClient = NULL;
	m_pMsgProtocol = NULL;
	m_bTcpIsGameServConnecting = false;
}

FTcpNetworkAgent::~FTcpNetworkAgent()
{
	m_pTcpGameSvrClient = NULL;
	m_pMsgProtocol = NULL;
	m_bTcpIsGameServConnecting = false;
}

void FTcpNetworkAgent::TcpClientCallBack(void* lpParam, const uint32 ulnEventType)
{
	FTcpNetworkAgent* pNetConnectAgent = (FTcpNetworkAgent*)lpParam;
	if (pNetConnectAgent == nullptr)
		return;

	switch (ulnEventType)
	{
	case enumServerConnectCreate:
	{
		pNetConnectAgent->SetGameServConnectState(true);
	}
	break;
	case enumServerConnectClose:
	{
		pNetConnectAgent->SetGameServConnectState(false);
		Log(TEXT("The Socket is disconnected!"));
	}
	break;
	}
}

bool FTcpNetworkAgent::Initialize(IMsgProtocol* pMsgProtocol)
{
	m_pMsgProtocol = pMsgProtocol;

	FNetLog::Init(&NetLog);
	return true;
}

void FTcpNetworkAgent::Shutdown()
{
	Close();
	delete this;
}

void FTcpNetworkAgent::Tick(float fDeltaTime)
{
	if (!IsConnecting())
		return;

	uint32 nSize = 0;
	const char* pBuffer = NULL;

	while (true)
	{
		if (!m_pTcpGameSvrClient || !m_bTcpIsGameServConnecting)
		{
			break;
		}

		pBuffer = (const char*)m_pTcpGameSvrClient->GetPackFromServer(nSize);
		if (!(pBuffer && nSize))
		{
			break;
		}

		ProcessGameServerData(pBuffer, nSize);
	}
}

bool FTcpNetworkAgent::Startup()
{
	DisconnectGameSvr();

	CreateTcpClient(m_pTcpGameSvrClient, MAX_SENDBUF, MAX_RECVBUF);


	if (!m_pTcpGameSvrClient)
	{
		Log(TEXT("CreateClient Failed!"));
		return false;
	}


	if (!m_pTcpGameSvrClient->Startup())
	{
		Log(TEXT("Net Startup Failed!"));
		return false;
	}

	m_pTcpGameSvrClient->RegisterMsgFilter((void*)this, &FTcpNetworkAgent::TcpClientCallBack);

	return true;
}

void FTcpNetworkAgent::Close()
{
	DisconnectGameSvr();

	if (m_pTcpGameSvrClient)
	{
		m_pTcpGameSvrClient->Cleanup();
		m_pTcpGameSvrClient->Release();
		m_pTcpGameSvrClient = NULL;
		m_pMsgProtocol = NULL;
	}
}

bool FTcpNetworkAgent::ConnectToGameSvr(const char* pIpAddress, unsigned short uPort)
{
	if (m_pTcpGameSvrClient == NULL)
	{
		Log(TEXT("m_pGameSvrClient == NULL!"));
		return false;
	}

	DisconnectGameSvr();

	if (!m_pTcpGameSvrClient->ConnectTo((char*)pIpAddress, uPort))
	{
		//Log(TEXT("Connect Failed! Ip = %s, port = %d"), ANSI_TO_TCHAR(pIpAddress), uPort);
		return false;
	}

	return true;
}

void FTcpNetworkAgent::DisconnectGameSvr()
{
	if (m_pTcpGameSvrClient)
	{
		m_pTcpGameSvrClient->Shutdown();
	}

	m_bTcpIsGameServConnecting = false;
}

/************************************************************************/
/*				发送消息到服务器，多线程处理消息，不影响主线程效率				*/
/************************************************************************/
bool FTcpNetworkAgent::SendDataToServer(const char* pBuffer, int32 length)
{
	if (!pBuffer || length <= 0)
		return false;

	if (!IsConnecting())
	{
		//Log(TEXT("The Socket is disconnected, Can't send a message!"));
		return false;
	}

	bool result = false;
	if (m_pTcpGameSvrClient && m_bTcpIsGameServConnecting)
	{
		result = m_pTcpGameSvrClient->SendPackToServer(pBuffer, length);
	}
	return result;
}


/************************************************************************/
/*				立即发送消息到服务器，一般特殊消息使用，游戏币相关等				*/
/************************************************************************/
bool FTcpNetworkAgent::SendMsgImmediately(const char* pBuffer, int32 length)
{
	if (SendDataToServer(pBuffer, length))
	{
		m_pTcpGameSvrClient->FlushData();
		return true;
	}
	return false;
}

bool FTcpNetworkAgent::IsConnecting()
{
	return m_bTcpIsGameServConnecting;
}

uint32 FTcpNetworkAgent::GetHeadSize()
{
	/* | pack_len | protoID | mark | */
	return sizeof(int32) + sizeof(int32) + sizeof(int16);
}

int32 FTcpNetworkAgent::PutPackHead(char* buf, int32 protocolID, int32 Length, int16 mark)
{
	int32 head_len = GetHeadSize();
	int32 iTotalLength = head_len + Length;
	*(int32*)buf = FSystemEndian::Swap(iTotalLength);
	*(int32*)(buf + sizeof(int32)) = FSystemEndian::Swap(protocolID);
	*(int16*)(buf + sizeof(int32) + sizeof(int32)) = FSystemEndian::Swap(mark);

	//Log(TEXT("head_len: %d, PB_len: %d, TotalLen: %d"), head_len, Length, iTotalLength);

	return iTotalLength;
}

void FTcpNetworkAgent::ProcessGameServerData(const char* pBuffer, unsigned int nSize)
{
	unsigned char* pMsg = (unsigned char*)pBuffer;

	if (m_pMsgProtocol == nullptr)
	{
		Log(TEXT("m_pMsgProtocol Is NULL!"));
		return;
	}

	if (pMsg == nullptr || nSize < 0)
	{
		Log(TEXT("Protocol Size Error!!!Recv[%d]"), nSize);
		return;
	}

	uint32 HeadLength = GetHeadSize();


	while (pMsg < (unsigned char*)(pBuffer + nSize))
	{

		int32 iTotalLength = FSystemEndian::Swap(*(int32*)pMsg);
		int32 ProtocolID = FSystemEndian::Swap(*(int32*)(pMsg + sizeof(int32)));
		//Log(TEXT("Protocol Size Error!!!Recv[%d]"), ProtocolID);
		FString Str = FString::FromInt(ProtocolID);
		int16 mark = FSystemEndian::Swap(*(int16*)(pMsg + sizeof(int32) + sizeof(int32)));
		uint32 MessageLen = iTotalLength - HeadLength;

		pMsg = (unsigned char*)(((char*)pMsg) + HeadLength);

		m_pMsgProtocol->ProcessMsg(ProtocolID, (void*)pMsg, MessageLen);

		pMsg = (unsigned char*)(((char*)pMsg) + MessageLen);

	}

}

