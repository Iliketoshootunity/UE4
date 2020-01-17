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

	//ȡ����ײ
	Character->SetActorEnableCollision(false);
	//ȡ���˶�
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
	//�ָ���ײ
	Character->SetActorEnableCollision(true);
	//�ָ��˶�
	Character->GetCharacterMovement()->SetComponentTickEnabled(true);
}

void UCSDeadState::OnPlayDeadFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if (CharacterInfo)
	{
		UWorldManager::Get()->RemoveCharacter(CharacterInfo->GetID());
	}

}

