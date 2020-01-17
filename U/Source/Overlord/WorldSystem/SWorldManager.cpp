// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldManager.h"
#include "OtherSystem/Loading/SLoadingManager.h"
#include "UISystem/SWidgetManager.h"
#include "UISystem/UI/SLoadingScreenWidget.h"
#include "SLoadingScreenTable.h"
#include "BaseSystem/GameUtil.h"
#include "BasicFunction.h"
#include "Engine/Texture2D.h"
#include "UISystem/UI/UILoadingScreen.h"
#include "Kismet/GameplayStatics.h"
#include "MapsTable.h"
#include "MessageDispatcher.h"
#include "Message.h"
#include "NpcResTable.h"
#include "TableDataDefine.h"
#include "GameFramework/PlayerController.h"
#include "NpcSystem/GameCharacter/CSGameMonster.h"
#include "NpcSystem/GameCharacter/CSGamePlayer.h"
#include "NpcSystem/Character/CSPlayer.h"
#include "SkillSystem/CSSkillComponent.h"
#include "Protoc/fight.pb.h"
#include "NpcSystem/Character/CSMonster.h"
#include "NpcSystem/GameCharacter/CSGameMainPlayer.h"
#include "Protoc/s2c.pb.h"
#include "SGameInstance.h"
#include "SkillSystem/SkillReciver/CSSkillReceiverInterface.h"
#include "SkillSystem/CSSkill.h"
#include "SkillSystem/CSComboClip.h"
#include "SkillSystem/SkiiResult/CSSkillResult.h"
#include "BaseSystem/FSM/FSMMachine.h"

void UWorldManager::Init()
{
	m_CurrentMap.Init();

	//绑定网络消息
	BindNetMessage();
}
void UWorldManager::Tick(float fDeltaTime)
{
	m_CurrentMap.Tick(fDeltaTime);
	UpdateAndCreateWaitList();
	TickMessage(fDeltaTime);
}
void UWorldManager::Shutdown()
{
	m_CurrentMap.Shutdown();
	//解绑网络消息
	UnBindNetMessage();
}



void UWorldManager::BindNetMessage()
{
	//视野包更新消息
	FMessageHandler UpdateViewMessageHangdler;
	UpdateViewMessageHangdler.BindUObject(this, &UWorldManager::OnResUpdateViewMessage);
	FMessageDispatcher::AddListener(FNetDefine::ResUpdateViewMessage, UpdateViewMessageHangdler, true);
	//玩家进入视野消息
	FMessageHandler PlayerEnteViewMessageHangdler;
	PlayerEnteViewMessageHangdler.BindUObject(this, &UWorldManager::OnResPlayerEnterViewMessage);
	FMessageDispatcher::AddListener(FNetDefine::ResPlayerEnterViewMessage, PlayerEnteViewMessageHangdler, true);
	//怪物进入视野消息
	FMessageHandler MonsterEnterViewMessageHangdler;
	MonsterEnterViewMessageHangdler.BindUObject(this, &UWorldManager::OnResMonsterEnterViewMessage);
	FMessageDispatcher::AddListener(FNetDefine::ResMonsterEnterViewMessage, MonsterEnterViewMessageHangdler, true);
	//场景物体退出视野消息
	FMessageHandler MapObjectExitViewMessageHangdler;
	MapObjectExitViewMessageHangdler.BindUObject(this, &UWorldManager::OnResMapObjectExitViewMessageMessage);
	FMessageDispatcher::AddListener(FNetDefine::ResMapObjectExitViewMessage, MapObjectExitViewMessageHangdler, true);
	//Combo消息
	FMessageHandler ComboMessageHangdler;
	ComboMessageHangdler.BindUObject(this, &UWorldManager::OnResComboNodeMessage);
	FMessageDispatcher::AddListener(FNetDefine::ResComboNodeMessage, ComboMessageHangdler, true);
	//ComboEnd消息
	FMessageHandler ComboEndMessageHangdler;
	ComboEndMessageHangdler.BindUObject(this, &UWorldManager::OnResComboEndNodeMessage);
	FMessageDispatcher::AddListener(FNetDefine::ResComboEndNodeMessage, ComboEndMessageHangdler, true);
	//攻击结果消息
	FMessageHandler FightResultMessageHangdler;
	FightResultMessageHangdler.BindUObject(this, &UWorldManager::OnResFightResultMessage);
	FMessageDispatcher::AddListener(FNetDefine::ResFightResultMessage, FightResultMessageHangdler, true);
	//移动消息
	FMessageHandler MoveMessageHandler;
	MoveMessageHandler.BindUObject(this, &UWorldManager::OnResMoveMessage);
	FMessageDispatcher::AddListener(FNetDefine::ResMoveMessage, MoveMessageHandler, true);
}

void UWorldManager::UnBindNetMessage()
{
	//视野包更新消息解绑
	FMessageHandler UpdateViewMessageHangdler;
	UpdateViewMessageHangdler.BindUObject(this, &UWorldManager::OnResUpdateViewMessage);
	FMessageDispatcher::RemoveListener(FNetDefine::ResUpdateViewMessage, UpdateViewMessageHangdler, true);
	//玩家进入视野解绑
	FMessageHandler PlayerEnteViewMessageHangdler;
	PlayerEnteViewMessageHangdler.BindUObject(this, &UWorldManager::OnResPlayerEnterViewMessage);
	FMessageDispatcher::RemoveListener(FNetDefine::ResPlayerEnterViewMessage, PlayerEnteViewMessageHangdler, true);
	//怪物进入视野解绑
	FMessageHandler MonsterEnterViewMessageHangdler;
	MonsterEnterViewMessageHangdler.BindUObject(this, &UWorldManager::OnResMonsterEnterViewMessage);
	FMessageDispatcher::RemoveListener(FNetDefine::ResMonsterEnterViewMessage, MonsterEnterViewMessageHangdler, true);
	//Combo消息
	FMessageHandler ComboMessageHangdler;
	ComboMessageHangdler.BindUObject(this, &UWorldManager::OnResComboNodeMessage);
	FMessageDispatcher::RemoveListener(FNetDefine::ResComboNodeMessage, ComboMessageHangdler, true);
	//ComboEnd消息
	FMessageHandler ComboEndMessageHangdler;
	ComboEndMessageHangdler.BindUObject(this, &UWorldManager::OnResComboEndNodeMessage);
	FMessageDispatcher::RemoveListener(FNetDefine::ResComboEndNodeMessage, ComboEndMessageHangdler, true);
	//攻击结果消息
	FMessageHandler FightResultMessageHangdler;
	FightResultMessageHangdler.BindUObject(this, &UWorldManager::OnResFightResultMessage);
	FMessageDispatcher::RemoveListener(FNetDefine::ResFightResultMessage, FightResultMessageHangdler, true);
	//移动消息
	FMessageHandler MoveMessageHandler;
	MoveMessageHandler.BindUObject(this, &UWorldManager::OnResMoveMessage);
	FMessageDispatcher::RemoveListener(FNetDefine::ResMoveMessage, MoveMessageHandler, true);

}

void UWorldManager::OnResUpdateViewMessage(FMessage* Message)
{
	if (Message != nullptr && Message->MessageData != nullptr && Message->MessageData->VoidRefDatas.Num() > 0)
	{
		mapV2::ResUpdateView* UpdateView = (mapV2::ResUpdateView*)(Message->MessageData->VoidRefDatas[0]);
		if (UpdateView)
		{
			for (int32 i = 0; i < UpdateView->addplayers_size(); i++)
			{
				void* PlayerInfo = (void*)(&(UpdateView->addplayers(i)));
				if (UpdateView->addplayers(i).rid() == TestMyID)
				{
					ProcessCharacterEnterView<UCSGameMainPlayer>(UpdateView->addplayers(i).rid(), PlayerInfo, ECharaterType::MainPlayer);
				}
				else
				{
					ProcessCharacterEnterView<UCSGamePlayer>(UpdateView->addplayers(i).rid(), PlayerInfo, ECharaterType::Player);
				}
			}
			for (int32 i = 0; i < UpdateView->addmonsters_size(); i++)
			{
				void* MonsterInfo = (void*)(&(UpdateView->addmonsters(i)));
				ProcessCharacterEnterView<UCSGameMonster>(UpdateView->addmonsters(i).lid(), MonsterInfo, ECharaterType::Monster);

				//测试
				//发送测试
				fightV2::FightTestContrllerNull FightTestContrllerNull;
				FightTestContrllerNull.set_targetid(UpdateView->addmonsters(i).lid());
				g_pGameInstance->SendMessage(199112, &FightTestContrllerNull);
			}
		}
	}
}



void UWorldManager::OnResPlayerEnterViewMessage(FMessage* Message)
{
	if (Message != nullptr && Message->MessageData != nullptr && Message->MessageData->VoidRefDatas.Num() > 0)
	{
		int64 ID = Message->MessageData->Int64Datas[0];
		void* PlayerInfo = Message->MessageData->VoidRefDatas[0];
		ProcessCharacterEnterView<UCSGamePlayer>(ID, PlayerInfo, ECharaterType::Player);
	}
}

void UWorldManager::OnResMonsterEnterViewMessage(FMessage* Message)
{
	if (Message != nullptr && Message->MessageData != nullptr && Message->MessageData->VoidRefDatas.Num() > 0)
	{
		int64 ID = Message->MessageData->Int64Datas[0];
		void* MonsterInfo = Message->MessageData->VoidRefDatas[0];
		ProcessCharacterEnterView<UCSGameMonster>(ID, MonsterInfo, ECharaterType::Monster);
	}
}


void UWorldManager::OnResMapObjectExitViewMessageMessage(FMessage* Message)
{
	if (Message != nullptr && Message->MessageData != nullptr && Message->MessageData->Int64Datas.Num() > 0)
	{
		int64 ID = Message->MessageData->Int64Datas[0];
		UCSGameCharacter* Character = GetCharacter(ID);
		if (Character)
		{
			if (Character->GetResManager()->GetActorUnit() && Character->GetResManager()->GetActorUnit()->GetMotionMachine()->GetCurrentStateID() != (int32)EMotionMachineState::Dead)
			{
				RemoveCharacter(ID);
			}

		}
	}
}

void UWorldManager::OnResComboNodeMessage(FMessage* Message)
{
	if (Message != nullptr && Message->MessageData != nullptr && Message->MessageData->VoidRefDatas.Num() > 0)
	{
		void* voidMessage = Message->MessageData->VoidRefDatas[0];
		fightV2::ComboNode* ComboNode = (fightV2::ComboNode*)(voidMessage);
		if (ComboNode)
		{
			if (MainPlayer != nullptr && ComboNode->id() == MainPlayer->GetID())
			{
				return;
			}
			int32 ComboId = ComboNode->combonode();
			int64 Yaw = ComboNode->dir();
			int32 X = ComboNode->x();
			int32 Y = ComboNode->y();
			int32 Z = ComboNode->z();
			UCSGameCharacter* Character = GetCharacter(ComboNode->id());
			if (Character == nullptr)
			{
				return;
			}
			ACSCharacter* CharacterUnit = Character->GetResManager()->GetActorUnit();
			if (CharacterUnit)
			{
				//设置动作起始朝向
				CharacterUnit->SetActorRotation(FRotator(CharacterUnit->GetActorRotation().Pitch, Yaw, CharacterUnit->GetActorRotation().Roll));
				//无视所有状态，强行切换成Idle
				CharacterUnit->ForcibleToIdle();
				FBasicFunction::Log(FString::FromInt(Yaw), true);
				if (Character->GetCharacterType() == ECharaterType::Monster)
				{
					Z = CharacterUnit->GetActorLocation().Z;
				}
				else if (Character->GetCharacterType() == ECharaterType::Player)
				{
					//覆盖操作信息
					float RockerX = ComboNode->rockerx();
					float RockerY = ComboNode->rockery();
					float ControllerYaw = ComboNode->controlleryaw();
					float TargetYaw = ComboNode->targetdir();
					float MaxRotate = ComboNode->maxrotate();
					ACSPlayer* PlayerUnit = Cast<ACSPlayer>(CharacterUnit);
					if (PlayerUnit)
					{
						PlayerUnit->CoverOperation(RockerX, RockerY, MaxRotate, Yaw, ControllerYaw, TargetYaw);
					}
				}
				//Mesh位移
				FVector OldPos = CharacterUnit->GetActorLocation();
				CharacterUnit->SetActorLocation(FVector(X, Y, Z));
				CharacterUnit->StartInterpMeshLocation(OldPos, OldPos, 0.05f);

				//释放技能
				if (CharacterUnit->GetSkillComponent())
				{
					CharacterUnit->GetSkillComponent()->RespondComboMessage(ComboId);
				}
			}

		}
	}
}

void UWorldManager::OnResComboEndNodeMessage(FMessage* Message)
{
	if (Message != nullptr && Message->MessageData != nullptr && Message->MessageData->VoidRefDatas.Num() > 0)
	{
		void* voidMessage = Message->MessageData->VoidRefDatas[0];
		fightV2::ComboEnd* ComboEndMessage = (fightV2::ComboEnd*)(voidMessage);
		if (ComboEndMessage != nullptr)
		{
			if (MainPlayer != nullptr && ComboEndMessage->id() == MainPlayer->GetID())
			{
				return;
			}
			UCSGameCharacter* Character = GetCharacter(ComboEndMessage->id());
			if (Character)
			{
				ACSCharacter* CharacterUnit = Character->GetResManager()->GetActorUnit();
				if (CharacterUnit)
				{
					if (CharacterUnit->IsAlive())
					{
						FVector Pos = FVector(ComboEndMessage->x(), ComboEndMessage->y(), ComboEndMessage->z());
						CharacterUnit->ForcibleToIdle();
						CharacterUnit->StopAllAnimMontages();
						CharacterUnit->MoveToTargetPosition(Pos, 600);
						//清算技能数据
						CharacterUnit->GetSkillComponent()->CurSkillFinished();
					}

				}
			}
		}
	}
}

void UWorldManager::OnResFightResultMessage(FMessage* Message)
{
	if (Message != nullptr && Message->MessageData != nullptr && Message->MessageData->VoidRefDatas.Num() > 0)
	{

		void* voidMessage = Message->MessageData->VoidRefDatas[0];
		fightV2::ResFightResult* FightResult = (fightV2::ResFightResult*)(voidMessage);
		if (FightResult)
		{
			int32 Count = FightResult->hurtlist_size();
			UCSGameCharacter* AttackCharacter = GetCharacter(FightResult->attackerid());
			if (AttackCharacter == nullptr)return;
			ACSCharacter* AttackCharacterUnit = AttackCharacter->GetResManager()->GetActorUnit();
			if (AttackCharacter == nullptr)return;
			int32 ComboId = FightResult->skillid();
			for (int32 i = 0; i < Count; i++)
			{
				fightV2::HurtTarget HurtMessage = FightResult->hurtlist()[i];
				UCSGameCharacter* Character = GetCharacter(HurtMessage.targetid());
				if (Character)
				{
					Character->SetHP(HurtMessage.hp());
					ACSCharacter* BeAttackCharacterUnit = Character->GetResManager()->GetActorUnit();
					if (HurtMessage.hp() <= 0)
					{
						if (BeAttackCharacterUnit)
						{
							BeAttackCharacterUnit->ForcibleToIdle();
							BeAttackCharacterUnit->ToDead(AttackCharacterUnit);
							return;
						}
					}
					//if (MainPlayer != nullptr && AttackCharacter->GetID() == MainPlayer->GetID())
					//{
					//	//已经在客户端先行了，暂时返回
					//	continue;
					//}

					if (BeAttackCharacterUnit)
					{
						BeAttackCharacterUnit->ForcibleToIdle();
						BeAttackCharacterUnit->GetSkillResult()->ProxyInit(AttackCharacterUnit, BeAttackCharacterUnit, FightResult, &HurtMessage);
						if (BeAttackCharacterUnit->GetSkillResult()->GetHurtClipName() == "")
						{
							return;
						}
						BeAttackCharacterUnit->ToHurt(BeAttackCharacterUnit->GetSkillResult()->GetHurtClipName());
					}
				}
			}
		}
	}
}

void UWorldManager::OnResMoveMessage(FMessage* Message)
{
	if (Message != nullptr && Message->MessageData != nullptr && Message->MessageData->VoidRefDatas.Num() > 0)
	{
		void* voidMessage = Message->MessageData->VoidRefDatas[0];
		moveV2::ResMove* MoveMessage = (moveV2::ResMove*)(voidMessage);
		if (MoveMessage)
		{
			int64 ID = MoveMessage->lid();
			if (MainPlayer != nullptr && ID == MainPlayer->GetID())
			{
				return;
			}
			UCSGameCharacter* Character = GetCharacter(ID);
			if (Character)
			{
				moveV2::ResMove* MoveMessage = (moveV2::ResMove*)(Message->MessageData->VoidRefDatas[0]);
				if (MoveMessage)
				{
					FVector TargetPos = FVector(MoveMessage->x(), MoveMessage->y(), MoveMessage->z());
					float Speed = MoveMessage->speed();
					ACSCharacter* CharacterUnit = Character->GetResManager()->GetActorUnit();
					if (CharacterUnit)
					{
						FVector NewTargetPos = CharacterUnit->CalNextTarget(TargetPos);
						if (Character->GetCharacterType() == ECharaterType::Player)
						{
							CharacterUnit->ToRun(NewTargetPos, Speed);
						}
						else if (Character->GetCharacterType() == ECharaterType::Monster)
						{
							//if (MoveMessage->isidle())
							//{
							//	Character->GetResManager()->GetActorUnit()->MoveToTargetPosition(TargetPos, Speed);
							//}
							//else
							//{
							//	ACSMonster* Monster = Cast<ACSMonster>(CharacterUnit);
							//	if (Monster)
							//	{
							//		FVector NewTargetPos = Monster->CalNextTarget(TargetPos);
							//		Monster->MoveToTargetPosition(NewTargetPos, Speed);
							//	}
							//}

							CharacterUnit->ToRun(NewTargetPos, Speed * 1.1f);


						}
					}
				}
			}
		}
	}
}

void UWorldManager::OnResDisconectInfo(FMessage* Message)
{
	if (Message != nullptr && Message->MessageData != nullptr && Message->MessageData->VoidRefDatas.Num() > 0)
	{
		void* voidMessage = Message->MessageData->VoidRefDatas[0];
		ResDisconnectInfo* DisconnectInfoMessage = (ResDisconnectInfo*)(voidMessage);
		int64 ID = DisconnectInfoMessage->userid();
		UCSGameCharacter* Character = GetCharacter(ID);
		if (Character)
		{
			//UGameUtil::DestroyActor(Character->GetResManager()->GetActorUnit());
			//RemoveCharacter(ID);
		}
	}
}

ACSCharacter* UWorldManager::CreateCharacterUnit(UCSGameCharacter* Character)
{
	if (Character == nullptr || Character->GetCharacterTypeData() == nullptr
		|| Character->GetResManager() == nullptr)return nullptr;
	bool IsOk = Character->GetResManager()->Init(Character->GetCharacterTypeData()->BlueprintName,
		Character->GetInitPos(),
		Character->GetInitRot());
	if (IsOk)
	{
		Character->GetResManager()->GetActorUnit()->Init(Character);
		if (Character->GetCharacterType() == ECharaterType::MainPlayer || Character->GetCharacterType() == ECharaterType::Player)
		{
			Character->GetResManager()->CreateWeapon();
		}
		return Character->GetResManager()->GetActorUnit();
	}
	return nullptr;
}

void UWorldManager::AddCharacter(ECharaterType CharacterType, UCSGameCharacter* BaseCharacter)
{
	int32 TypeKey = (int32)CharacterType;
	if (BaseCharacter == nullptr || GetCharacter(BaseCharacter->GetID()) != nullptr)
		return;
	CharacterMap.Add(BaseCharacter->GetID(), BaseCharacter);
	if (!CharacterMapList.Contains(TypeKey))
	{
		TArray<UCSGameCharacter*>  CharacterList;
		CharacterMapList.Add(TypeKey, CharacterList);
	}
	CharacterMapList[TypeKey].Add(BaseCharacter);
}

void UWorldManager::RemoveCharacter(int64 Id)
{
	if (MainPlayer != nullptr && MainPlayer->GetID() == Id)
	{
		//主角直接返回
		return;
	}

	UCSGameCharacter* CharacterTemp = GetCharacter(Id);
	if (CharacterTemp != nullptr)
	{
		int32 CT = (int32)(CharacterTemp->GetCharacterType());
		if (CharacterMapList.Contains(CT))
		{
			CharacterMapList[CT].Remove(CharacterTemp);
		}
		CharacterMap.Remove(Id);
		if (WaitCreateCharacterList.Contains(CharacterTemp))
		{
			WaitCreateCharacterList.Remove(CharacterTemp);
		}
		CharacterTemp->GetResManager()->DestoryUnit();
	}


}

void UWorldManager::UpdateAndCreateWaitList()
{
	if (bLoadMapFinshed)
	{
		int MaxCount = WaitCreateCharacterList.Num();
		if (MaxCount <= 0)return;
		if (MaxCount > CREATECHARACTERCOUNTEVERYFRAME)
		{
			MaxCount = CREATECHARACTERCOUNTEVERYFRAME;
		}
		for (int32 i = 0; i < MaxCount; i++)
		{
			UCSGameCharacter* CharacterTemp = WaitCreateCharacterList[WaitCreateCharacterList.Num() - 1];
			if (CharacterTemp)
			{
				ACSCharacter* C = CreateCharacterUnit(CharacterTemp);
				//测试，这是主角，与控制器绑定
				if (CharacterTemp->GetID() == TestMyID)
				{
					UWorld* W = GetWorld();
					if (W == nullptr)
						return;
					APlayerController* PC = UGameplayStatics::GetPlayerController((const UObject*)(W), 0);
					if (PC != nullptr)
					{
						PC->Possess(C);
						g_pGameInstance->SetPlayerCharacter(Cast<ACSPlayer>(C));
					}
				}
			}
			WaitCreateCharacterList.RemoveAt(WaitCreateCharacterList.Num() - 1);
		}
	}
}

void UWorldManager::TickMessage(float DeltaTime)
{
	if (bLoadMapFinshed)
	{
		if (MainPlayer && MainPlayer->GetResManager()->GetActorUnit())
		{
			ACSPlayer* PlayerUnit = Cast<ACSPlayer>(MainPlayer->GetResManager()->GetActorUnit());
			if (PlayerUnit)
			{
				//PlayerUnit->SendMoveMessage(DeltaTime);
			}
		}
	}
}

UCSGameCharacter* UWorldManager::GetCharacter(int64 Id)
{
	UCSGameCharacter* Character = nullptr;
	if (MainPlayer != nullptr && MainPlayer->GetID() == Id)
	{
		Character = MainPlayer;
		return Character;
	}
	if (CharacterMap.Num() > 0 && CharacterMap.Contains(Id))
	{
		Character = CharacterMap.FindRef(Id);
	}
	return Character;
}

bool UWorldManager::LoadMap(int32 MapID, bool bShowLoading /*= true*/)
{
	if (MapID == m_CurrentMap.GetMapID())
	{
		return false;
	}
	bLoadMapFinshed = false;
	if (bShowLoading)
	{
		ShowLoadingScreen();
	}
	const FMapsTableData* MapInfo = MapsTable::Get()->GetData(MapID);
	if (!MapInfo) return false;
	UWorld* pCurWorld = GetWorld();
	if (pCurWorld == nullptr)
		return false;
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.ExecutionFunction = TEXT("OnLoadLevelFinished");
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.Linkage = 1;
	UGameplayStatics::LoadStreamLevel(pCurWorld, FName(*(MapInfo->MapName)), true, false, LatentActionInfo);
	return true;
}

bool UWorldManager::UnloadMap()
{
	return m_CurrentMap.UnloadMap();
}

void UWorldManager::OnLoadLevelFinished(int32 LinkID)
{
	//打开场景完成后，模拟进度条播放完成
	UUILoadingScreen* pWidget = Cast<UUILoadingScreen>(FWidgetManager::Get()->GetWidget(EUIType::UI_LoadingScreen));
	if (pWidget != nullptr)
	{
		pWidget->OnLoadMapFinshed();
	}
}

void UWorldManager::ShowLoadingScreen()
{
	UUILoadingScreen* pWidget = Cast<UUILoadingScreen>(FWidgetManager::Get()->ShowLoadingScreen());
	if (pWidget == nullptr)
		return;

	FMapsTableData* pMapTableData = m_CurrentMap.GetTableData();
	if (pMapTableData)
	{
		//背景图片更改
		//TODO
	}
	pWidget->SetProgressPercent(0.0f);
	pWidget->SetAnimFinishedEvent(FOnAnimFinished::CreateUObject(this, &UWorldManager::HideLoadingScreen));
	//模拟播放到80%
	pWidget->OnLoadMapStart();
}

void UWorldManager::HideLoadingScreen()
{
	//发送更新视野消息
	FWidgetManager::Get()->HideLoadingScreen();
	bLoadMapFinshed = true;
}



