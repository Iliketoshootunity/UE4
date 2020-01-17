// Fill out your copyright notice in the Description page of Project Settings.


#include "CSSkill.h"
#include "Animation/AnimMontage.h"
#include "BasicFunction.h"
#include "CSComboClip.h"
#include "CSSkillComponent.h"
#include "ComboClipTable.h"
#include "SkillTable.h"
#include "Animation/AnimInstance.h"
#include "BaseSystem/DataStructure/DoubleLinkedList.h"
#include "NpcSystem/Character/CSCharacter.h"
#include "NpcSystem/Character/CSMainPlayer.h"


void UCSSkill::Init(UCSSkillComponent* SkillManager, const FSkillTableData* SkillData)
{
	this->SkillManager = SkillManager;
	this->Owner = Cast<ACSCharacter>(SkillManager->GetOwner());
	this->SkillData = SkillData;

	//创建双向链表
	ComboList = new FMapList();
}


void UCSSkill::Tick()
{
	if (GetCurComboClip())
	{
		GetCurComboClip()->Tick();
	}
}

void UCSSkill::Play()
{
	if (SkillData)
	{
		if (SkillData->FirstComboMontage != "")
		{
			//创建第一个片段
			const FComboClipTableData* ClipData = ComboClipTable::Get()->GetData(SkillData->FirstComboClipId);
			FComboJumpLineData Data;
			OnCreateAndPlayComboClip((FComboClipTableData*)ClipData, SkillData->FirstComboMontage, Data);
		}
	}
}

void UCSSkill::Reset()
{
	int32 Count = ComboList->Size();
	for (size_t i = 0; i < Count; i++)
	{
		UObject* content = ComboList->Back();
		if (content)
		{
			UCSComboClip* Clip = Cast<UCSComboClip>(content);
			if (Clip)
			{
				Clip->Reset();
				ComboList->Pop();
			}
		}
	}
}


void UCSSkill::OnCreateAndPlayComboClip(FComboClipsTableData* ClipData, FString MontageName, FComboJumpLineData LineData)
{
	UCSComboClip* NewClip = nullptr;
	FString componentBPFilePath = "/Game/Blueprints/";
	componentBPFilePath.Append("ComboClip.ComboClip_C");
	UClass* templateClass = LoadClass<UCSComboClip>(this, *componentBPFilePath);
	if (templateClass == nullptr)
	{
		FBasicFunction::Log("UComboClip Blueprints not load");
		return;
	}
	NewClip = NewObject<UCSComboClip>(this, templateClass);
	if (NewClip)
	{
		if (ClipData == nullptr)
		{
			FBasicFunction::Log("USkill::Init() FComboClipsTableData not found", true);
			return;
		}
		NewClip->Init(ClipData, MontageName, LineData);
		NewClip->SetCreateAndPlayComboClipDelegate(FCreateAndPlayComboClipDelegate::CreateUObject(this, &UCSSkill::OnCreateAndPlayComboClip));
		NewClip->SetChargeInterruptedDelegate(FChargeInterruptedDelegate::CreateUObject(this, &UCSSkill::OnChargeInterrupted));
		ComboList->InsertToTail(NewClip);
	}
	if (Owner)
	{
		bool IsOk = Owner->ToSkill(ClipData->Path);
		//同步消息
		SendComboMessage();
	}
}


void UCSSkill::OnChargeInterrupted()
{
	if (Owner)
	{
		Owner->StopAllAnimMontages();
		SkillManager->CurSkillFinished();
		//发送Combo消息
		SendComboEndMessage();
		Owner->ToIdle();
	}
}

UCSComboClip* UCSSkill::GetCurComboClip()
{
	UObject* content = ComboList->Back();
	if (content)
	{
		UCSComboClip* Clip = Cast<UCSComboClip>(content);
		if (Clip)
		{
			return Clip;
		}
	}
	return nullptr;
}


void UCSSkill::SendComboMessage()
{
	if (Owner->IsMainPlayer())
	{
		if (ComboList != nullptr)
		{

			UObject* Obj = ComboList->Back();
			UCSComboClip* LastCombo = Cast<UCSComboClip>(Obj);
			if (LastCombo)
			{
				int32 ComboId = LastCombo->GetClipData()->ID;
				ACSMainPlayer* Player = Cast<ACSMainPlayer>(Owner);
				if (Owner)
				{
					Player->SendComboMessage(ComboId);
				}
			}

		}
	}

}

void UCSSkill::SendComboEndMessage()
{
	if (Owner->IsMainPlayer())
	{
		if (ComboList != nullptr)
		{
			ACSMainPlayer* Player = Cast<ACSMainPlayer>(Owner);
			if (Owner)
			{
				Player->SendComboEndMessage();
			}
		}
	}
}

bool UCSSkill::RespondComboMessage(int32 ComboId)
{
	const FComboClipTableData* ClipData = ComboClipTable::Get()->GetData(ComboId);
	if (ClipData == nullptr)return false;
	FComboJumpLineData Data;
	OnCreateAndPlayComboClip((FComboClipTableData*)ClipData, ClipData->Path, Data);
	return true;
}

void UCSSkill::Destroy()
{
	Reset();
	SkillManager = nullptr;
	if (ComboList)
	{
		delete ComboList;
		ComboList = nullptr;
	}
}

