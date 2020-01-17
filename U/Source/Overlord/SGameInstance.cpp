// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"
#include "Ticker.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NetSystem/SProtocolProcess.h"
#include "SGameSingleton.h"
#include "GameConfig.h"
#include "OtherSystem/Loading/SLoadingManager.h"
#include "BasicFunction.h"
#include "Protoc/c2s.pb.h"
#include "Networkdef.h"
#include "SystemEndian.h"
#include "ITcpNetworkAgent.h"
#include "WorldSystem/SWorldManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "BaseSystem/Ext/MathfExtLibrary.h"
#include "MessageDispatcher.h"
#include "NpcSystem/Character/CSPlayer.h"
#include "Protoc/user.pb.h"


OVERLORD_API USGameInstance* g_pGameInstance = nullptr;


USGameInstance::USGameInstance()
{
	g_pGameInstance = this;

}

USGameInstance::~USGameInstance()
{
	g_pGameInstance = nullptr;

}

void USGameInstance::Init()
{
	UGameSingleton::Get()->Init();

	InitNetwork();

	FTickerDelegate TickDelegate = FTickerDelegate::CreateUObject(this, &USGameInstance::Tick);
	m_TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);

	Super::Init();
}

void USGameInstance::Shutdown()
{

	UWorldManager::Get()->Shutdown();

	if (m_pNetConnectAgent)
	{
		m_pNetConnectAgent->Shutdown();
		m_pNetConnectAgent = nullptr;
	}

	g_pGameInstance = nullptr;

	Super::Shutdown();
}

void USGameInstance::StartGameInstance()
{

	//判断当前地图是否已经加载，引擎启动会加载默认地图
	//UWorld* pCurWorld = GetWorld();

	//FString CurMapName = pCurWorld->GetName();

	Super::StartGameInstance();

	UWorld* pCurWorld = GetWorld();

	FString CurMapName = pCurWorld->GetName();

	if (CurMapName == TEXT("Pregame"))
	{
		UGameSingleton::Get()->StartGame();
		//由于Editor模式下，Movices不运行，直接跳过
		ULoadingManager::Get()->OnLoadingFinished(ELoadingMark::Mark_Launcher);
	}


	//ULoadingManager::Get()->OnLoadingFinished(ELoadingMark::Mark_Launcher);
}

#if WITH_EDITOR
FGameInstancePIEResult USGameInstance::InitializeForPlayInEditor(int32 PIEInstanceIndex, const FGameInstancePIEParameters& Params)
{
	FGameInstancePIEResult retVal = Super::InitializeForPlayInEditor(PIEInstanceIndex, Params);

	return retVal;
}

FGameInstancePIEResult USGameInstance::StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params)
{

	//判断当前地图是否已经加载，引擎启动会加载默认地图
	UWorld* pCurWorld = GetWorld();

	FString CurMapName = pCurWorld->GetName();

	if (CurMapName == TEXT("Pregame"))
	{
		UGameSingleton::Get()->StartGame();
		//由于Editor模式下，Movices不运行，直接跳过
		ULoadingManager::Get()->OnLoadingFinished(ELoadingMark::Mark_Launcher);
	}
	//UGameSingleton::Get()->StartGame();
	////由于Editor模式下，Movices不运行，直接跳过
	//ULoadingManager::Get()->OnLoadingFinished(ELoadingMark::Mark_Launcher);


	FGameInstancePIEResult retVal = Super::StartPlayInEditorGameInstance(LocalPlayer, Params);

	return retVal;
}
#endif


void USGameInstance::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void USGameInstance::InitNetwork()
{

	const FGameCfgData* pData = SGameConfig::Get()->GetData();
	if (!pData) return;

	if (!m_pNetConnectAgent)
	{
		if ((m_pNetConnectAgent = GetTcpNetworkAgent()) == NULL)
			return;
	}


	m_pNetConnectAgent->Initialize(&g_ProtocolProcess);

	//连接资源服务器
	if (pData->bIsUseTcp)
	{
		ConnectServer(pData->szTcpIP, pData->nTcpPort);
	}

	if (pData->bIsUseUdp)
	{
		ConnectServer(pData->szUdpIP, pData->nUdpPort, false);
	}


}

bool USGameInstance::ConnectServer(FString IP, int32 nPort, bool IsTcp /*= true*/)
{
	if (!m_pNetConnectAgent) return false;

	if (IsTcp)
	{
		if (!m_pNetConnectAgent->Startup())
		{
			FBasicFunction::Logs(TEXT("TCP Net Initialize Failed!"));
			return false;
		}

		if (!m_pNetConnectAgent->ConnectToGameSvr((char*)TCHAR_TO_UTF8(*IP), nPort))
		{
			return false;
		}
	}

	return true;
}

void USGameInstance::DisConnectServer()
{
	if (!m_pNetConnectAgent) return;

	if (m_pNetConnectAgent->IsConnecting())
	{
		m_pNetConnectAgent->DisconnectGameSvr();
	}
}

bool USGameInstance::SendMessage(int32 protocolID, void* pMessage, int16 mark /*= 0*/)
{
	if (!m_pNetConnectAgent) return false;

	google::protobuf::Message* pPB = (google::protobuf::Message*)pMessage;
	int32 iHeadLength = m_pNetConnectAgent->GetHeadSize();

	int32 iPBLength = pPB->ByteSize();
	if (iHeadLength + iPBLength >= RB_MAXPACK)
		return false;

	char szSendBuf[RB_MAXPACK] = { 0 };

	int32 iTotalLength = m_pNetConnectAgent->PutPackHead(szSendBuf, protocolID, iPBLength, mark);
	pPB->SerializePartialToArray(szSendBuf + iHeadLength, RB_MAXPACK - iHeadLength);


	bool IsOk = m_pNetConnectAgent->SendMsgImmediately(szSendBuf, iTotalLength);

	return IsOk;

}


void USGameInstance::SendPlayerWalkMessage(float DeltaTime)
{
	//if (PlayerCharacter)
	//{
	//	PlayerCharacter->SendMoveMessage(DeltaTime);
	//}
}

void USGameInstance::SendOtherCharacterInfoMessage()
{
	//请求怪物信息
	test::ReqUpdateView UpdateView;
	SendMessage(199109, &UpdateView);
	//请求玩家信息
	SendMessage(199110, &UpdateView);
}

void USGameInstance::SetPlayerCharacter(ACSPlayer* Player)
{
	PlayerCharacter = Player;
}

bool USGameInstance::Tick(float fDeltaTime)
{

	if (m_pNetConnectAgent)
	{
		static float lastTime = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles());
		float nowTime = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles());
		if (nowTime - lastTime > 500.f)
		{

			lastTime = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles());

			FPing PingCmd;
			PingCmd.set_ctime(lastTime);
			SendMessage(c2s_ping, &PingCmd);

		}
		m_pNetConnectAgent->Tick(fDeltaTime);

		//主角移动消息
		SendPlayerWalkMessage(fDeltaTime);
	}

	UGameSingleton::Get()->Tick(fDeltaTime);

	UWorldManager::Get()->Tick(fDeltaTime);

	FMessageDispatcher::Tick(fDeltaTime);

	return true;
}
