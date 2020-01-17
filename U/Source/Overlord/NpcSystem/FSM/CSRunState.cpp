// Fill out your copyright notice in the Description page of Project Settings.


#include "CSRunState.h"
#include "../Character/CSCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Character/CSMainPlayer.h"
#include "../Other/CSCharacterController.h"
#include "TimerManager.h"
#include "BaseSystem/FSM/FSMMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BasicFunction.h"

UCSRunState::UCSRunState()
{

}


void UCSRunState::SetIsSprint(bool IsSprint)
{
	if (IsSprint)
	{
		GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
		bIsSprint = IsSprint;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
		FTimerDelegate TimerCallback = FTimerDelegate::CreateUObject(this, &UCSRunState::SetNotSprintDelay);
		GetWorld()->GetTimerManager().SetTimer(SprintTimerHandle, TimerCallback, 0.5f, false);
	}
}

void UCSRunState::SetNotSprintDelay()
{
	bIsSprint = false;
}

void UCSRunState::Enter()
{
	Character->StopAnimMontage(StopRunMontage);
	Character->StopAllAnimMontages();
	if (CharacterType != ECharaterType::MainPlayer)
	{
		Character->MoveToTargetPosition(RunTarget, MoveSpeed);
	}
	Character->HideWeapon(1);
}

void UCSRunState::Update(float DeltaTime)
{
	if (CharacterType != ECharaterType::MainPlayer)
	{
		//查看是否达到目标
		float Dis = UKismetMathLibrary::Vector_Distance(RunTarget, Character->GetActorLocation());
		if (Dis < 100)
		{
			Character->ToIdle();
		}

	}
	else
	{
		ACSMainPlayer* Player = Cast<ACSMainPlayer>(Character);
		if (Player)
		{
			Player->Move();
		}
	}
}

void UCSRunState::Exit()
{
	if (CharacterType == ECharaterType::MainPlayer || CharacterType == ECharaterType::Player)
	{
		if (bIsSprint)
		{
			FString StopRunPath = "/Game/Character/Hero/Anim/Montage/RunToIdle.RunToIdle";
			FOnMontageEnded End;
			Character->PlayMontage(StopRunPath, StopRunMontage, End);
		}

	}


}
