// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSCharacterState.h"
#include "Animation/AnimInstance.h"
#include "CSDeadState.generated.h"

class UAnimMontage;
/**
 *
 */
UCLASS()
class OVERLORD_API UCSDeadState : public UCSCharacterState
{
	GENERATED_BODY()
public:
	UCSDeadState();
protected:
	FOnMontageEnded EndDelagate;
public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;
protected:
	virtual void OnPlayDeadFinished(UAnimMontage* Montage, bool bInterrupted);		//动画结束回调;
	float Timer;
};
