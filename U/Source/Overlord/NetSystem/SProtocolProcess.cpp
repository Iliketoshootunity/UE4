// Fill out your copyright notice in the Description page of Project Settings.

#include "SProtocolProcess.h"
#include "Kismet/GameplayStatics.h"
#include "SGameInstance.h"
#include "BaseSystem/GameUtil.h"
#include "OtherSystem/Pregame/SPregameManager.h"
#include "WorldSystem/SWorldManager.h"
#include "Protoc/s2c.pb.h"
#include "Protoc/move.pb.h"
#include "WorldSystem/SWorldManager.h"
#include "Protoc/test.pb.h"
#include "Protoc/map.pb.h"
#include "google/protobuf/repeated_field.h"
#include "UISystem/SWidgetManager.h"
#include "Protoc/fight.pb.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "MessageData.h"
#include "MessageDispatcher.h"
#include "NpcSystem/GameCharacter/CSGameCharacter.h"
#include "NpcSystem/GameCharacter/CSCharacterRes.h"
#include "NpcSystem/Character/CSPlayer.h"
#include "NpcSystem/Character/CSCharacter.h"
#include "Protoc/user.pb.h"



OVERLORD_API SProtocolProcess g_ProtocolProcess;

SProtocolProcess::SProtocolProcess()
{
	//FPlatformMemory::Memzero(ProcessFunc, sizeof(ProcessFunc));

	// 注册协议
	//ProcessFunc[s2c_ping] = &SProtocolProcess::s2cPing;
	//ProcessFunc[s2c_login] = &SProtocolProcess::s2cLogin;
	// 注册协议
	m_FuncMap.Empty();
	m_FuncMap.Add(s2c_ping, &SProtocolProcess::s2cPing);
	m_FuncMap.Add(s2c_login, &SProtocolProcess::s2cLogin);
	m_FuncMap.Add(1021, &SProtocolProcess::s2cDisconectInfo);
	//Combo
	m_FuncMap.Add(69015, &SProtocolProcess::s2cComboNodeMessage);
	m_FuncMap.Add(69017, &SProtocolProcess::s2cComboEndMessage);
	m_FuncMap.Add(199111, &SProtocolProcess::ResRoleInfo);
	//Map
	m_FuncMap.Add(67001, &SProtocolProcess::ResUpdateViewMessage);
	m_FuncMap.Add(67003, &SProtocolProcess::ResMonsterEnterViewMessage);
	m_FuncMap.Add(67002, &SProtocolProcess::ResPlayerEnterViewMessage);
	m_FuncMap.Add(67008, &SProtocolProcess::ResMapObjectExitViewMessage);
	//Move
	m_FuncMap.Add(68002, &SProtocolProcess::ResMoveMessage);
	//Fight
	m_FuncMap.Add(69002, &SProtocolProcess::ResFightResult);
	//Heart
	m_FuncMap.Add(1010, &SProtocolProcess::ResHeartMessage);
	/*ProcessFunc[s2c_syncworld] = &SProtocolProcess::s2cSyncworld;
	ProcessFunc[s2c_rolelist] = &SProtocolProcess::s2cRoleLsit;
	ProcessFunc[s2c_synclocalplayer] = &SProtocolProcess::s2cSynclocalplayer;
	ProcessFunc[s2c_syncmincharacter] = &SProtocolProcess::s2cMinCharacter;
	ProcessFunc[s2c_synccharacter] = &SProtocolProcess::s2cCharacter;*/

}

SProtocolProcess::~SProtocolProcess()
{
}

bool SProtocolProcess::ProcessMsg(int32 usProtocolType, void* pMsgData, int32 length)
{
	if (usProtocolType == 67001)
	{
		FString Str = "";

	}

	//FBasicFunction::Logs(TEXT(" SProtocolProcess ProcessMsg Start %d"), usProtocolType);

	if (!m_FuncMap.Contains(usProtocolType) || m_FuncMap[usProtocolType] == NULL)
	{
		return false;
	}

	//(this->*ProcessFunc[usProtocolType])((uint8*)pMsgData, length);

	//FBasicFunction::Logs(TEXT("ProcessMsg Start %d"), usProtocolType);

	(this->*m_FuncMap[usProtocolType])((uint8*)pMsgData, length);

	//FBasicFunction::Logs(TEXT("ProcessMsg End"));

	return true;
}

void SProtocolProcess::s2cPing(uint8* pMsg, int32 length)
{
	if (!pMsg) return;

	FPingRet PingRet;
	PingRet.ParseFromArray(pMsg, length);
	FBasicFunction::Logs(TEXT("s2cPing Server Time = %d"), PingRet.stime());

}

void SProtocolProcess::s2cLogin(uint8* pMsg, int32 length)
{
	ResLogin ResLogin;
	ResLogin.ParseFromArray(pMsg, length);

	int64 Result = ResLogin.userid();

	TestMyID = Result;
	//UGameplayStatics::OpenLevel((const UObject*)pCurWorld, "MainCity");
	//UWorldManager::Get()->LoadMap(2, false);

	UWorld* pCurWorld = g_pGameInstance->GetWorld();

	if (pCurWorld == nullptr)
		return;
	FWidgetManager::Get()->Hide(EUIType::UI_Login);
	//UGameplayStatics::OpenLevel((const UObject*)pCurWorld, "LevelTest");	


	UWorldManager::Get()->LoadMap(3, true);


	//请求时间戳
	userV2::ReqHeart RepHeart;
	RepHeart.set_clienttime(0);
	g_pGameInstance->SendMessage(1009, &RepHeart);

	//UObject* MapAssert = UGameUtil::SyncLoadAsset(TEXT("/Game/Maps/L_level_001/main_level_01_BuiltData.main_level_01_BuiltData"));
	//UGameplayStatics::OpenLevel((const UObject*)pCurWorld, TEXT("NewMap"));

	//   SLoginMsg* pLogin = (SLoginMsg*)pMsg;
	//   if (pLogin == NULL || pLogin->usLenght != sizeof(SLoginMsg))
	//       return;

	//   // 处理登陆返回逻辑
	//   unsigned int nRet = pLogin->Param;
	   //if (nRet == 1)
	   //{
	   //	OOO
	   //}

	//   FBasicFunction::Logs(TEXT("Server Login Ret: %d !!!"), nRet);

}

void SProtocolProcess::s2cSyncworld(uint8* pMsg, int32 length)
{
	/*FSyncworldMsg* pSyncworldMsg = (FSyncworldMsg*)pMsg;
	if (pSyncworldMsg == NULL)
		return;

	UWorldManager::Get()->LoadMap(pSyncworldMsg->nMapID);*/
}

void SProtocolProcess::s2cRoleLsit(uint8* pMsg, int32 length)
{
	//   SRoleListMsg* pRoleList = (SRoleListMsg*)pMsg;
	//   if (pRoleList == NULL)
	//       return;

	// //  FName IMapName = pRoleList->RoleCount <= 0  ? TEXT("CreateRole") : TEXT("CreateRole");

	//   if (pRoleList->RoleCount <= 0)
	//   {
	   //	UPregameManager::Get()->ShowCreateRole();
	//   }
	   //else
	   //{
	   //	//FSingletonManager::Get()->GetSingleton<UPregameManager>()->ShowSelectRole();
	   //	UPregameManager::Get()->ShowSelectRole();
	   //}


	 //  else
	 //  {
	 //      //IMapName = TEXT("Map_SelRole");

	 //      RoleBaseMsg* pRoleBase = (RoleBaseMsg*)pRoleList->RoleList;

	 //      // 保存RoleNameList 
	 //      //for ( int i = 0; i<pRoleList->RoleCount; ++i)
	 //      //{

	 //      //}

	 //      // 先直接进游戏
	 //      CSelectPlayerMsg SelRoleCmd;
	 //      SelRoleCmd.usLenght = sizeof(CSelectPlayerMsg);
	 //      SelRoleCmd.usProtocol = c2s_selectplayer;
	 //      FPlatformMemory::Memcpy(SelRoleCmd.RoleName, pRoleBase[0].RoleName, sizeof(SelRoleCmd.RoleName));

	 //      g_pGameInstance->GetNet()->TcpSendDataToServer(&SelRoleCmd, false);
	 //  }

	 //  if (g_pGameInstance->GetWorld() != nullptr)
	 //  {
		   //FLatentActionInfo LatentInfo;
	 //      UGameplayStatics::LoadStreamLevel(g_pGameInstance->GetWorld(), IMapName,true,false, LatentInfo);
	 //  }
	 //  else
	 //  {
		   //FLatentActionInfo LatentInfo;
	 //      UGameplayStatics::LoadStreamLevel(GWorld, IMapName, true, false, LatentInfo);
	 //  }
}

void SProtocolProcess::s2cSynclocalplayer(uint8* pMsg, int32 length)
{
	/*FSyncLocalPlayerMsg* pData = (FSyncLocalPlayerMsg*)pMsg;
	if (!pData)
		return;

	int32 idx =UCharacterManager::Get()->SearchID(pData->uID);

	if (!idx)
	{
		return;
	}

	UCSGameCharacter* pCharacter = UCharacterManager::Get()->GetCharacter(idx);
	if (!pCharacter)
	{
		return;
	}

	pCharacter->m_HairType = pData->HairType;
	pCharacter->m_FaceType = pData->FaceType;
	pCharacter->m_ShoulderType = pData->ShoulderType;
	pCharacter->m_BodyType = pData->BodyType;
	pCharacter->m_BeltType = pData->BeltType;
	pCharacter->m_LegType = pData->LegType;
	pCharacter->m_LWeaponType = pData->LWeaponType;
	pCharacter->m_RWeaponType = pData->RWeaponType;
	pCharacter->m_HorseType = pData->HorseType;*/

	//pCharacter->GetResManager()->Init()
}

void SProtocolProcess::s2cMinCharacter(uint8* pMsg, int32 length)
{
	//FSyncMinCharacter* pMinCharacter = (FSyncMinCharacter*)pMsg;
	//if (!pMinCharacter)
	//	return;

	//int32 nIndex = UCharacterManager::Get()->SearchID(pMinCharacter->uID);

	//if (!nIndex)
	//{
	//	// 向服务器请求同步这个NPC的全部数据
	//	if (!UCharacterManager::Get()->IsCharRequestExist(pMinCharacter->uID))
	//	{
	//		FS2CCharacterRequestCommand CharacterRequest;
	//		CharacterRequest.usProtocol = c2s_requestcharacter;
	//		CharacterRequest.uID = pMinCharacter->uID;
	//		CharacterRequest.Kind = pMinCharacter->Kind;
	//		CharacterRequest.usLenght = sizeof(FS2CCharacterRequestCommand);

	//		if (g_pGameInstance)
	//			g_pGameInstance->GetNet()->TcpSendDataToServer(&CharacterRequest);

	//		UCharacterManager::Get()->InsertCharRequest(pMinCharacter->uID);
	//	}
	//}
	//else
	//{
	//	UCSGameCharacter* pCharacter = UCharacterManager::Get()->GetCharacter(nIndex);
	//	if (pCharacter == nullptr || (pCharacter && !pCharacter->IsValidLowLevel()))
	//	{
	//		return;
	//	}
	//}
}

void SProtocolProcess::s2cCharacter(uint8* pMsg, int32 length)
{
	/*SyncCharacter* pData = (SyncCharacter*)pMsg;
	if (!pData)
		return;

	int32 nIndex = UCharacterManager::Get()->SearchID(pData->uID);
	if (!nIndex)
	{
		nIndex = UCharacterManager::Get()->Add(pData->iTableID, pData->Location, pData->Rotator);
	}

	UCSGameCharacter* pCharacter = UCharacterManager::Get()->GetCharacter(nIndex);
	if (!pCharacter)
		return;

	pCharacter->m_uKind = pData->Kind;*/
}


void SProtocolProcess::s2cDisconectInfo(uint8* pMsg, int32 length)
{
	ResDisconnectInfo DisconnectInfo;
	DisconnectInfo.ParseFromArray(pMsg, length);
	//发送消息
	UMessagedata* MessageData = NewObject<UMessagedata>();
	MessageData->VoidRefDatas.Add((void*)(&DisconnectInfo));
	FMessageDispatcher::SendMessage(FNetDefine::ResDisconectInfo, MessageData);
}

void SProtocolProcess::s2cComboNodeMessage(uint8* pMsg, int32 length)
{
	fightV2::ComboNode ComboNode;
	ComboNode.ParseFromArray(pMsg, length);
	UMessagedata* MessageData = NewObject<UMessagedata>();
	MessageData->VoidRefDatas.Add((void*)(&ComboNode));
	FMessageDispatcher::SendMessage(FNetDefine::ResComboNodeMessage, MessageData);

}

void SProtocolProcess::s2cComboEndMessage(uint8* pMsg, int32 length)
{
	fightV2::ComboEnd ComboEnd;
	ComboEnd.ParseFromArray(pMsg, length);
	int64 Id = ComboEnd.id();
	//发送消息
	UMessagedata* MessageData = NewObject<UMessagedata>();
	MessageData->VoidRefDatas.Add((void*)(&ComboEnd));
	FMessageDispatcher::SendMessage(FNetDefine::ResComboEndNodeMessage, MessageData);
}


void SProtocolProcess::ResRoleInfo(uint8* pMsg, int32 length)
{
	//玩家登陆角色返回
	test::RoleInfo RoleInfo;
	RoleInfo.ParseFromArray(pMsg, length);
	int64 RoleID = RoleInfo.rid();
	TestMyID = RoleID;
	UWorldManager::Get()->TestMyID = RoleID;
}

void SProtocolProcess::ResUpdateViewMessage(uint8* pMsg, int32 length)
{
	UWorld* pCurWorld = g_pGameInstance->GetWorld();
	if (pCurWorld == nullptr)return;
	mapV2::ResUpdateView UpdateView;
	UpdateView.ParseFromArray(pMsg, length);
	//发送消息
	UMessagedata* MessageData = NewObject<UMessagedata>();
	MessageData->VoidRefDatas.Add((void*)(&UpdateView));
	FMessageDispatcher::SendMessage(FNetDefine::ResUpdateViewMessage, MessageData);
}

void SProtocolProcess::ResMonsterEnterViewMessage(uint8* pMsg, int32 length)
{
	mapV2::ResMonsterEnterView MonsterEnterVie;
	MonsterEnterVie.ParseFromArray(pMsg, length);
	mapV2::RoundMonsterInfo MonsterInfo = MonsterEnterVie.monster();
	//发送消息
	UMessagedata* MessageData = NewObject<UMessagedata>();
	MessageData->Int64Datas.Add(MonsterInfo.lid());
	MessageData->VoidRefDatas.Add((void*)(&MonsterInfo));
	FMessageDispatcher::SendMessage(FNetDefine::ResMonsterEnterViewMessage, MessageData);

}

void SProtocolProcess::ResPlayerEnterViewMessage(uint8* pMsg, int32 length)
{
	UWorld* pCurWorld = g_pGameInstance->GetWorld();
	if (pCurWorld == nullptr)return;
	mapV2::ResPlayerEnterView PlayerEnterView;
	PlayerEnterView.ParseFromArray(pMsg, length);
	mapV2::RoundPlayerInfo PlayerInfo = PlayerEnterView.player();
	//发送消息
	UMessagedata* MessageData = NewObject<UMessagedata>();
	MessageData->Int64Datas.Add(PlayerInfo.rid());
	MessageData->VoidRefDatas.Add((void*)(&PlayerInfo));
	FMessageDispatcher::SendMessage(FNetDefine::ResPlayerEnterViewMessage, MessageData);
}

void SProtocolProcess::ResMapObjectExitViewMessage(uint8* pMsg, int32 length)
{
	mapV2::ResMapObjectExitView MapObjectExitView;
	MapObjectExitView.ParseFromArray(pMsg, length);
	//发送消息
	UMessagedata* MessageData = NewObject<UMessagedata>();
	MessageData->Int64Datas.Add(MapObjectExitView.lid());
	FMessageDispatcher::SendMessage(FNetDefine::ResMapObjectExitViewMessage, MessageData);
}

void SProtocolProcess::ResMoveMessage(uint8* pMsg, int32 length)
{
	moveV2::ResMove MoveMessage;
	MoveMessage.ParseFromArray(pMsg, length);
	float Speed = MoveMessage.action() == 1 ? 300 : 600;
	if (MoveMessage.lid() == TestMyID)return;
	//发送消息
	UMessagedata* MessageData = NewObject<UMessagedata>();
	MessageData->VoidRefDatas.Add((void*)(&MoveMessage));
	FMessageDispatcher::SendMessage(FNetDefine::ResMoveMessage, MessageData);
}

void SProtocolProcess::ResFightResult(uint8* pMsg, int32 length)
{
	fightV2::ResFightResult FightResult;
	FightResult.ParseFromArray(pMsg, length);
	//发送消息
	UMessagedata* MessageData = NewObject<UMessagedata>();
	MessageData->VoidRefDatas.Add((void*)(&FightResult));
	FMessageDispatcher::SendMessage(FNetDefine::ResFightResultMessage, MessageData);
}

void SProtocolProcess::ResHeartMessage(uint8* pMsg, int32 length)
{
	userV2::ResHeart Heart;
	Heart.ParseFromArray(pMsg, length);
	int64 NowTime = Heart.nowtime();
	int64 ClientTime = Heart.clienttime();
}


