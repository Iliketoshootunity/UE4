// Fill out your copyright notice in the Description page of Project Settings.


#include "CSSkillResult.h"
#include "Kismet/KismetMathLibrary.h"
#include "ComboClipTable.h"
#include "BaseSystem/Ext/ActorInterpMovementComponent.h"
#include "SkillSystem/CSSkill.h"
#include "SkillSystem/CSComboClip.h"
#include "HitPerformace.h"
#include "SGameInstance.h"
#include "SkillSystem/CSSkillComponent.h"
#include "BaseSystem/GameUtil.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "NpcSystem/Character/CSCharacter.h"
#include "NpcSystem/GameCharacter/CSGameCharacter.h"

UCSSkillResult::UCSSkillResult()
{

}

void UCSSkillResult::LocalInit(FSkillReceiveInfo ReceiveInfo)
{
	this->ReceiveInfo = ReceiveInfo;

	FVector AttackerPos = ReceiveInfo.Attacker->GetActorLocation();
	FVector BeAttackerPos = ReceiveInfo.BeAttacker->GetActorLocation();
	KeepDistance = 0;
	KeepDistanceTime = 0;
	int32 HitID = ReceiveInfo.HitCheckInfo.HitPerformaceId;
	HitData = HitPerformace::Get()->GetData(HitID);
	if (HitData == nullptr)return;
	FVector AdjustPosition = BeAttackerPos;
	if (HitData->PushDistance > 0)
	{
		KeepDistance = HitData->PushDistance;
		KeepDistanceTime = 0.05f;
	}
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(BeAttackerPos, AttackerPos);
	float Distance = UKismetMathLibrary::Vector_Distance(FVector(AttackerPos.X, AttackerPos.Y, 0), FVector(BeAttackerPos.X, BeAttackerPos.Y, 0));
	if (Distance < KeepDistance)
	{
		AdjustPosition = UKismetMathLibrary::GetForwardVector(FRotator(0, ReceiveInfo.Attacker->GetActorRotation().Yaw, 0)) * KeepDistance + BeAttackerPos;
	}

	UWorld* CurWorld = g_pGameInstance->GetWorld();
	if (CurWorld)
	{
		UPrimitiveComponent* HitComponent = ReceiveInfo.HitResult.Component.Get();
		if (HitComponent)
		{
			//判断材质
			//TODO
			UParticleSystem* Particle = UGameUtil::LoadClass<UParticleSystem>(HitData->HitMeatFX);
			if (Particle)
			{
				{
					UParticleSystemComponent* Test = UGameplayStatics::SpawnEmitterAtLocation((const UObject*)CurWorld, Particle, ReceiveInfo.HitResult.Location, FRotator::ZeroRotator);
					/*			FString HitFXRotate = HitData->HitFXRotate;
								TArray<FString> HitFXRotateArr;
								HitFXRotate.ParseIntoArray(HitFXRotateArr, TEXT("_"), true);
								if (HitFXRotateArr.Num() == 3)
								{
									float Pitch = FCString::Atof(*HitFXRotateArr[0]);
									float Yaw = FCString::Atof(*HitFXRotateArr[1]);
									float Roll = FCString::Atof(*HitFXRotateArr[2]);
									FRotator HitRotate = FRotator(Pitch, Yaw, Roll);
									Test->SetWorldRotation(HitRotate);
								}*/
					Test->SetWorldRotation(ReceiveInfo.HitCheckInfo.HitFXRotate);

				}
			}

			//摄像机震动
			UClass* CameraShake = UGameUtil::LoadClass<UClass>(HitData->CameraShake);
			if (CameraShake)
			{
				UGameplayStatics::GetPlayerCameraManager((const UObject*)CurWorld, 0)->PlayCameraShake(CameraShake, 1, ECameraAnimPlaySpace::CameraLocal);
			}
		}

		//发送攻击请求
		fightV2::FightRequest FightRequest;
		FightRequest.set_skillid(ReceiveInfo.Skill->GetID());
		FightRequest.set_comboid(ReceiveInfo.ComboClip->GetId());
		ACSCharacter* Test = Cast<ACSCharacter>(ReceiveInfo.BeAttacker);
		FightRequest.set_targetid(Test->GetCharacterInfo()->GetID());
		FightRequest.set_x(AttackerPos.X);
		FightRequest.set_y(AttackerPos.Y);
		FightRequest.set_z(AttackerPos.Z);
		FightRequest.set_movex(AttackerPos.X);
		FightRequest.set_movey(AttackerPos.Y);
		FightRequest.set_movez(AttackerPos.Z);
		FightRequest.set_targetx(AdjustPosition.X);
		FightRequest.set_targety(AdjustPosition.Y);
		FightRequest.set_targetz(AdjustPosition.Z);
		FightRequest.set_targetdir(Rot.Yaw);
		FightRequest.set_controllerid(ReceiveInfo.Attacker->GetCharacterInfo()->GetID()); //测试，控制者ID
		FightRequest.set_hitperformaceid(HitID);

		g_pGameInstance->SendMessage(69001, &FightRequest);


	}
}

void UCSSkillResult::ProxyInit(ACSCharacter* InAttacker, ACSCharacter* InBeAttacker, fightV2::ResFightResult* InFightResult, fightV2::HurtTarget* InHurtTarget)
{
	HurtClipName = "";
	if (InAttacker == nullptr || InBeAttacker == nullptr)return;
	//判断此次攻击效果是否生效
	//规则暂时这样
	bool IsOk = false;
	if (InFightResult->attackerid() == InFightResult->controllerid())
	{
		IsOk = true;
	}
	Attacker = InAttacker;
	BeAttacker = InBeAttacker;
	FVector AttackPos = FVector(InFightResult->movex(), InFightResult->movey(), Attacker->GetActorLocation().Z);
	FVector TargetPos = FVector(InHurtTarget->x(), InHurtTarget->y(), BeAttacker->GetActorLocation().Z);
	FVector AttackerPos = Attacker->GetActorLocation();
	FVector BeAttackerPos = BeAttacker->GetActorLocation();
	FRotator TargetRot = FRotator::ZeroRotator;
	if (Attacker->GetCharacterInfo()->GetCharacterType() == ECharaterType::Monster)
	{
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(BeAttackerPos, AttackerPos);
		TargetRot = FRotator(0, Rot.Yaw, 0);
	}
	else
	{
		TargetRot = FRotator(0, InHurtTarget->targetdir(), 0);
		if (Attacker->GetCharacterInfo()->GetCharacterType() == ECharaterType::Player)
		{
			//Attacker->GetInterpMovement()->StartMoveToTarget(AttackPos, 0.05f);
			FVector OldPos = Attacker->GetActorLocation();
			Attacker->SetActorLocation(AttackPos);
			Attacker->StartInterpMeshLocation(OldPos, OldPos, 0.05f);

		}
	}
	if (IsOk)
	{
		BeAttacker->SetActorRotation(TargetRot);
		FVector OldPos = BeAttacker->GetActorLocation();
		BeAttacker->SetActorLocation(TargetPos);
		BeAttacker->StartInterpMeshLocation(OldPos, OldPos, 0.05f);
	}
	int32 SkillId = InFightResult->skillid();
	int32 ComboId = InFightResult->comboid();
	int32 HitId = InFightResult->hitperformaceid();

	HitData = HitPerformace::Get()->GetData(HitId);
	if (HitData)
	{
		if (IsOk)
		{
			//暂时必播受击动画
			if (HitData->HitType == 0)
			{
				//普通受击动画
				HurtClipName = InBeAttacker->GetCharacterInfo()->GetCharacterTypeData()->NormalHurtMontage;
			}
			else if (HitData->HitType == 1)
			{
				//击飞受击动画
				HurtClipName = InBeAttacker->GetCharacterInfo()->GetCharacterTypeData()->DiaupHurtMontage;
			}
			else if (HitData->HitType == 2)
			{
				//大受击动画
				HurtClipName = InBeAttacker->GetCharacterInfo()->GetCharacterTypeData()->BigHurtMontage;
			}
		}

		//顿帧
		if (HitData->AttackerPauseDelayFrame > 0 && HitData->AttackerPauseDurationFrame > 0)
		{
			Attacker->StopAniFrame(HitData->AttackerPauseDelayFrame, HitData->AttackerPauseDurationFrame);
		}

		if (HitData->SuffererPauseDelayFrame > 0 && HitData->SuffererPauseDurationFrame > 0)
		{
			BeAttacker->StopAniFrame(HitData->SuffererPauseDelayFrame, HitData->SuffererPauseDurationFrame);
		}
	}

}


