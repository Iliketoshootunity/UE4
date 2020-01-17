// Fill out your copyright notice in the Description page of Project Settings.


#include "CSDeadState.h"
#include "../Character/CSCharacter.h"
#include "../GameCharacter/CSGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseSystem/GameUtil.h"
#include "WorldSystem/SWorldManager.h"
#include "Animation/AnimMontage.h"

UCSDeadState::UCSDeadState()
{

}

void UCSDeadState::Enter()
{
	FString DeadMontage = CharacterInfo->GetCharacterTypeData()->DeadMontage;
	UAnimMontage* CurDeadMontage = nullptr;
	EndDelagate.BindUObject(this, &UCSDeadState::OnPlayDeadFinished);
	Character->PlayMontage(DeadMontage, CurDeadMontage, EndDelagate);

	//取消碰撞
	Character->SetActorEnableCollision(false);
	//取消运动
	Character->GetCharacterMovement()->SetComponentTickEnabled(false);
}

void UCSDeadState::Update(float DeltaTime)
{
	Timer += DeltaTime;
	if (Timer >= 1.5f)
	{
		if (CharacterInfo)
		{
			UWorldManager::Get()->RemoveCharacter(CharacterInfo->GetID());
		}
	}
}

void UCSDeadState::Exit()
{
	//恢复碰撞
	Character->SetActorEnableCollision(true);
	//恢复运动
	Character->GetCharacterMovement()->SetComponentTickEnabled(true);
}

void UCSDeadState::OnPlayDeadFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if (CharacterInfo)
	{
		UWorldManager::Get()->RemoveCharacter(CharacterInfo->GetID());
	}

}

