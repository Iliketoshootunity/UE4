// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSCharacterState.h"
#include "Animation/AnimMontage.h"
#include "CSRollState.generated.h"

/**
 *
 */
UCLASS()
class OVERLORD_API UCSRollState : public UCSCharacterState
{
	GENERATED_BODY()
public:
	UCSRollState();
protected:
	FString RollPath;
	FOnMontageEnded EndDelagate;
public:
	FORCEINLINE void SetRollPath(FString NewRollPath) { RollPath = NewRollPath; }
public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;
protected:
	virtual void OnPlayRollFinished(UAnimMontage* Montage, bool bInterrupted);		//动画结束回调;
};
