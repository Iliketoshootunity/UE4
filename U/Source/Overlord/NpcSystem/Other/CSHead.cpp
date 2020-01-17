// Fill out your copyright notice in the Description page of Project Settings.


#include "CSHead.h"
#include "MessageDispatcher.h"
#include "Message.h"
#include "BaseSystem/GameUtil.h"
#include "WidgetComponent.h"
#include "UISystem/UI/UIHead.h"
#include "UserWidget.h"
#include "SGameInstance.h"
#include "../Character/CSCharacter.h"
#include "../GameCharacter/CSGameCharacter.h"
#include "../GameCharacter/CSCharacterRes.h"
#include "WorldSystem/SWorldManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Character/CSMainPlayer.h"

UCSHead::UCSHead()
{
	BindMessage();
	bActorUnitInstance = false;
}

UCSHead::~UCSHead()
{
	UnBindMessage();
}

void UCSHead::Init(UCSGameCharacter* InBaseCharacter)
{
	BaseCharacter = InBaseCharacter;
	if (BaseCharacter == nullptr)return;
	FCSCharacterRes* Res = BaseCharacter->GetResManager();
	if (Res == nullptr)return;
	ACSCharacter* ActorUnit = Res->GetActorUnit();
	if (ActorUnit == nullptr)return;
	HeadWidgetComponent = ActorUnit->GetHeadWidget();
	if (HeadWidgetComponent == nullptr)return;
	HeadWidget = Cast<UUIHead>(HeadWidgetComponent->GetUserWidgetObject());
	if (HeadWidget == nullptr)return;
	HeadWidget->Init();
	bActorUnitInstance = true;
	//设置UI
	FString CharacterName = BaseCharacter->GetCharacterName();
	HeadWidget->SetCharacterName(CharacterName);
	float MaxHp = BaseCharacter->GetMaxHp();
	float Hp = BaseCharacter->GetHp();
	HeadWidget->UpdateMaxHp(MaxHp);
	HeadWidget->UpdateHp(Hp);
}

void UCSHead::BindMessage()
{
	//HP变动绑定
	FMessageHandler HpMessageHangdler;
	HpMessageHangdler.BindUObject(this, &UCSHead::OnHpChange);
	FMessageDispatcher::AddListener(FMessageDefine::CharacterHPChange, HpMessageHangdler, true);
	//最大HP变动绑定
	FMessageHandler MaxHpMessageHandler;
	MaxHpMessageHandler.BindUObject(this, &UCSHead::OnMaxHpChange);
	FMessageDispatcher::AddListener(FMessageDefine::CharacterMaxHPChange, MaxHpMessageHandler, true);
	//角色名字设置绑定
	FMessageHandler SetNameMessageHandler;
	SetNameMessageHandler.BindUObject(this, &UCSHead::OnSetCharateName);
	FMessageDispatcher::AddListener(FMessageDefine::SetCharacterName, SetNameMessageHandler, true);

}

void UCSHead::UnBindMessage()
{
	//HP变动解绑
	FMessageHandler HpMessageHangdler;
	HpMessageHangdler.BindUObject(this, &UCSHead::OnHpChange);
	FMessageDispatcher::RemoveListener(FMessageDefine::CharacterHPChange, HpMessageHangdler);
	//最大HP变动解绑
	FMessageHandler MaxHpMessageHandler;
	MaxHpMessageHandler.BindUObject(this, &UCSHead::OnMaxHpChange);
	FMessageDispatcher::RemoveListener(FMessageDefine::CharacterMaxHPChange, MaxHpMessageHandler, true);
	//角色名字设置解绑
	FMessageHandler SetNameMessageHandler;
	SetNameMessageHandler.BindUObject(this, &UCSHead::OnSetCharateName);
	FMessageDispatcher::RemoveListener(FMessageDefine::SetCharacterName, SetNameMessageHandler, true);
}

void UCSHead::OnHpChange(FMessage* Message)
{
	if (Message == nullptr || Message->MessageData == nullptr || Message->MessageData->Int64Datas.Num() <= 0)return;
	int64 ID = Message->MessageData->Int64Datas[0];
	int64 OldHp = Message->MessageData->Int64Datas[1];
	int64 NewHp = Message->MessageData->Int64Datas[2];
	float DifHp = OldHp - NewHp;
	if (BaseCharacter != nullptr && HeadWidget != nullptr)
	{
		if (BaseCharacter->GetID() == ID)
		{
			if (DifHp > 0)
			{
				//显示播放血量动画(测试版本)
				if (BaseCharacter != nullptr && BaseCharacter->GetResManager() && BaseCharacter->GetResManager()->GetActorUnit())
				{
					BaseCharacter->GetResManager()->GetActorUnit()->SpawnDamgeHP(DifHp);
				}
			}
			HeadWidget->UpdateHp(NewHp);
			if (NewHp <= 0)
			{
				HeadWidgetComponent->SetHiddenInGame(true);
			}
		}
	}
}

void UCSHead::OnMaxHpChange(FMessage* Message)
{
	if (Message == nullptr || Message->MessageData == nullptr || Message->MessageData->Int64Datas.Num() <= 0)return;
	int64 ID = Message->MessageData->Int64Datas[0];
	int64 OldMaxHp = Message->MessageData->Int64Datas[1];
	int64 NewMaxHp = Message->MessageData->Int64Datas[2];
	if (BaseCharacter != nullptr && HeadWidget != nullptr)
	{
		if (BaseCharacter->GetID() == ID)
		{
			HeadWidget->UpdateMaxHp(NewMaxHp);
		}
	}
}

void UCSHead::OnSetCharateName(FMessage* Message)
{
	if (Message == nullptr || Message->MessageData == nullptr || Message->MessageData->FStringDatas.Num() <= 0 || Message->MessageData->Int64Datas.Num() <= 0)return;
	int64 ID = Message->MessageData->Int64Datas[0];
	FString CN = Message->MessageData->FStringDatas[0];
	if (BaseCharacter != nullptr && HeadWidget != nullptr)
	{
		if (BaseCharacter->GetID() == ID)
		{
			HeadWidget->SetCharacterName(CN);
		}
	}
}

void UCSHead::LookAtCamera()
{
	if (BaseCharacter == nullptr || HeadWidgetComponent == nullptr)return;
	if (BaseCharacter->GetCharacterType() == ECharaterType::MainPlayer)return;
	FVector StartPos = HeadWidgetComponent->GetComponentLocation();
	UCSGameCharacter* GC = UWorldManager::Get()->GetMainPlayer();
	if (GC)
	{
		ACSCharacter* A = GC->GetResManager()->GetActorUnit();
		if (A)
		{
			ACSMainPlayer* p = Cast<ACSMainPlayer>(A);
			if (p)
			{
				FVector EndPos = p->GetMainPlayrCameraPos();
				FRotator Rot = UKismetMathLibrary::FindLookAtRotation(StartPos, EndPos);
				HeadWidgetComponent->SetWorldRotation(FRotator(0, Rot.Yaw, 0));
			}
		}
	}
}

void UCSHead::Tick(float DeltaTime)
{
	LookAtCamera();
}
