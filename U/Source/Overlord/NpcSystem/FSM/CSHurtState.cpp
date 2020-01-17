// Fill out your copyright notice in the Description page of Project Settings.


#include "CSHurtState.h"
#include "../Character/CSCharacter.h"
#include "../GameCharacter/CSGameCharacter.h"
#include "SGameInstance.h"

UCSHurtState::UCSHurtState()
{
	HurtPath.Empty();
}

void UCSHurtState::Enter()
{
	Character->HideWeapon(1);
	Character->ReleaseSkillData();
	UAnimMontage* CurHurtMontage = nullptr;
	EndDelagate.BindUObject(this, &UCSHurtState::OnPlayHurtFinished);
	Character->PlayMontage(HurtPath, CurHurtMontage, EndDelagate);
}

void UCSHurtState::Update(float DeltaTime)
{

}

void UCSHurtState::Exit()
{
	EndDelagate.Unbind();
	Character->ResetAniFrame();
	Character->SetAnimRootMotionTranslationScale(1);

	Character->SkillController = nullptr;		//暂时受击完之后控制者置空
}

void UCSHurtState::OnPlayHurtFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		Character->ToIdle();
		//发送测试
		fightV2::FightTestContrllerNull FightTestContrllerNull;
		FightTestContrllerNull.set_targetid(CharacterInfo->GetID());
		g_pGameInstance->SendMessage(199112 , &FightTestContrllerNull);
	}
}
