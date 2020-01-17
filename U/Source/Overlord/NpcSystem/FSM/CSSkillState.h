// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSCharacterState.h"
#include "Animation/AnimMontage.h"
#include "CSSkillState.generated.h"

/**
 *
 */
UCLASS()
class OVERLORD_API UCSSkillState : public UCSCharacterState
{
	GENERATED_BODY()
public:
	UCSSkillState();
protected:
	FString ComboPath;
	FOnMontageEnded EndDelagate;
	bool bIsCharge;
public:
	FORCEINLINE void SetComboPath(FString NewComboPath) { ComboPath = NewComboPath; }
public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;
protected:
	virtual void OnPlayComboFinished(UAnimMontage* Montage, bool bInterrupted);		//技能动画动画结束回调;
};
