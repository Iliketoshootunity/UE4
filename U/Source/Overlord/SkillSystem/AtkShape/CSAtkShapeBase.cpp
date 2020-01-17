// Fill out your copyright notice in the Description page of Project Settings.


#include "CSAtkShapeBase.h"
#include "NpcSystem/Character/CSCharacter.h"
#include "../CSComboClip.h"
#include "../SkillReciver/CSSkillReceiverInterface.h"
#include "NpcSystem/GameCharacter/CSGameCharacter.h"
#include "SGameInstance.h"
#include "Components/PrimitiveComponent.h"
ACSAtkShapeBase::ACSAtkShapeBase()
{

}

void ACSAtkShapeBase::Init(ACSCharacter* InOwner, UCSSkill* InSkill, UCSComboClip* InClip, FHitCheckInfo InHitChekInfo)
{
	AtkOwner = InOwner;
	Skill = InSkill;
	ComboClip = InClip;
	ComboId = ComboClip->GetId();
	AtkIndex = 1;
	HasBeAtker.Empty();
	HitChekInfo = InHitChekInfo;
}

void ACSAtkShapeBase::AtkCheck()
{
	if (ComboClip == nullptr || AtkOwner == nullptr)return;
	TArray<FHitResult> TriggetList = BP_AtkCheck();
	if (AtkOwner)
	{
		TMap<AActor*, TArray<FHitResult>> HasBeAtkerTemp;
		for (int32 i = 0; i < TriggetList.Num(); i++)
		{
			UPrimitiveComponent* HitComponent = TriggetList[i].Component.Get();
			AActor* BeAtkActor = HitComponent->GetOwner();
			ICSSkillReceiverInterface* SkillReceiver = Cast<ICSSkillReceiverInterface>(BeAtkActor);
			if (SkillReceiver)
			{
				if (HasBeAtker.Contains(BeAtkActor))continue;
				if (!HasBeAtkerTemp.Contains(BeAtkActor))
				{
					TArray<FHitResult> HasTriggerList;
					HasBeAtkerTemp.Add(BeAtkActor, HasTriggerList);
				}
				HasBeAtkerTemp[BeAtkActor].Add(TriggetList[i]);
			}

		}
		for (TMap<AActor*, TArray<FHitResult>>::TIterator It(HasBeAtkerTemp); It; ++It)
		{
			AActor* BeAtkActor = It->Key;
			ICSSkillReceiverInterface* SkillReceiver = Cast<ICSSkillReceiverInterface>(BeAtkActor);
			if (SkillReceiver)
			{
				//查找一个优先级最先的选项
				//TODO
				FHitResult P = It->Value[0];
				FSkillReceiveInfo ReceiveInfo;
				ReceiveInfo.Attacker = AtkOwner;
				ReceiveInfo.BeAttacker = BeAtkActor;
				ReceiveInfo.Skill = Skill;
				ReceiveInfo.ComboClip = ComboClip;
				ReceiveInfo.HitCheckInfo = HitChekInfo;
				ReceiveInfo.HitResult = P;
				SkillReceiver->Execute_ReceiverSkill(BeAtkActor, ReceiveInfo);
				HasBeAtker.Add(BeAtkActor);
				//暂时
				AtkOwner->AtkTarget =Cast<ACSCharacter>(BeAtkActor);
			}
		}
	}
}
