// Fill out your copyright notice in the Description page of Project Settings.


#include "CSIdleState.h"
#include "../Character/CSCharacter.h"
#include "BaseSystem/FSM/FSMMachine.h"
#include "../Other/CSCharacterController.h"

UCSIdleState::UCSIdleState()
{

}

void UCSIdleState::Enter()
{
	//如果是上一个状态是技能，则进入战斗待机
	int32 OldStateId = Machine->GetOldStateID();
	if (OldStateId == (int32)EMotionMachineState::Skill)
	{
		bFightIdle = true;
		FightTime = 3;
		FightTimer = 0;
		//FString FightIdlePath = "/Game/Character/Hero/Anim/Montage/FightIdle.FightIdle";
		//FOnMontageEnded End;
		//Character->PlayMontage(FightIdlePath, FightIdle, End);
	}
	else
	{
		bFightIdle = false;
	}
}

void UCSIdleState::Update(float DeltaTime)
{
	if (bFightIdle)
	{
		FightTimer += DeltaTime;
		if (FightTimer > FightTime)
		{
			bFightIdle = false;
			Character->HideWeapon(1);
			FString FightIdle_ToIdlePath = "/Game/Character/Hero/Anim/Montage/FightIdle_ToIdle.FightIdle_ToIdle";
			FOnMontageEnded End;
			Character->PlayMontage(FightIdle_ToIdlePath, FightIdle_ToIdle, End);
		}
	}
}

void UCSIdleState::Exit()
{
	bFightIdle = false;
	int32 CurStateId = Machine->GetCurrentStateID();
	if (CurStateId == (int32)EMotionMachineState::Skill)
	{
		bFightIdle = true;
		FightTime = 3;
		FightTimer = 0;
	}
}
