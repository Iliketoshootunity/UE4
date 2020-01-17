// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSCharacterState.h"
#include "Animation/AnimInstance.h"
#include "CSHurtState.generated.h"


UCLASS()
class OVERLORD_API UCSHurtState : public UCSCharacterState
{
	GENERATED_BODY()
public:
	UCSHurtState();
protected:
	FString HurtPath;
	FOnMontageEnded EndDelagate;
public:
	FORCEINLINE void SetHurtPath(FString NewHurtPath) { HurtPath = NewHurtPath; }
public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;
protected:
	virtual void OnPlayHurtFinished(UAnimMontage* Montage, bool bInterrupted);		//动画结束回调;
};
