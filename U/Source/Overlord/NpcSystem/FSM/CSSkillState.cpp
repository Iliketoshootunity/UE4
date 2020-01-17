// Fill out your copyright notice in the Description page of Project Settings.


#include "CSSkillState.h"
#include "../Character/CSCharacter.h"
#include "Animation/AnimInstance.h"
#include "../Other/CSCharacterDefine.h"
#include "../Character/CSMainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SkillSystem/CSComboClip.h"
#include "SkillSystem/CSComboJumpLine.h"
#include "SkillSystem/CSSkill.h"
#include "SkillSystem/CSSkillComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

UCSSkillState::UCSSkillState()
{
	ComboPath.Empty();
}

void UCSSkillState::Enter()
{
	bIsCharge = false;
	Character->ShowWeapon();
	Character->StopMove();
	Character->PrepareComboClip();
	EndDelagate.BindUObject(this, &UCSSkillState::OnPlayComboFinished);
	UAnimMontage* CurComMontage = nullptr;
	Character->PlayMontage(ComboPath, CurComMontage, EndDelagate);
	UCSComboClip* CurClip = Character->GetSkillComponent()->GetCurSkill()->GetCurComboClip();
	if (CurClip->IsCharge())
	{
		bIsCharge = true;
		FName PName = FName("OutBloomSwitch");
		Character->GetMaterialInstanceDynamic()->SetScalarParameterValue(PName, 2);
	}
}

void UCSSkillState::Update(float DeltaTime)
{

}

void UCSSkillState::Exit()
{
	EndDelagate.Unbind();
	Character->ResetAniFrame();
	Character->SetAnimRootMotionTranslationScale(1);
	FName PName = FName("OutBloomSwitch");
	Character->GetMaterialInstanceDynamic()->SetScalarParameterValue(PName, 0);
}

void UCSSkillState::OnPlayComboFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		//为了所有端的技能数据一致，以下为主端逻辑，代理端走ComboNodeEnd消息
		if (CharacterType == ECharaterType::MainPlayer || CharacterType == ECharaterType::Monster)
		{
			Character->ReleaseSkillData();
			Character->StopMove();
			//发送ComboNodeEnd消息
			ACSMainPlayer* Player = Cast<ACSMainPlayer>(Character);
			if (Player)
			{
				Player->SendComboEndMessage();
			}
			Character->ToIdle();
		}
	}
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
