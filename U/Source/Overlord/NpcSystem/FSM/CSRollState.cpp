// Fill out your copyright notice in the Description page of Project Settings.


#include "CSRollState.h"
#include "../Character/CSCharacter.h"

UCSRollState::UCSRollState()
{
}

void UCSRollState::Enter()
{
	Character->HideWeapon(1);
	Character->SetCanRoll(false);
	Character->ReleaseSkillData();
	EndDelagate.BindUObject(this, &UCSRollState::OnPlayRollFinished);
	UAnimMontage* CurRollMontage = nullptr;
	Character->PlayMontage(RollPath, CurRollMontage, EndDelagate);
}

void UCSRollState::Update(float DeltaTime)
{

}

void UCSRollState::Exit()
{
	EndDelagate.Unbind();
	Character->ResetAniFrame();
	Character->SetAnimRootMotionTranslationScale(1);
	Character->SetCanRoll(true);
}

void UCSRollState::OnPlayRollFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		Character->ToIdle();
	}
}
