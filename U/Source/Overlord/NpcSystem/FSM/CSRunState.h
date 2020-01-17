// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSCharacterState.h"
#include "Engine/EngineTypes.h"
#include "CSRunState.generated.h"

class UAnimMontage;

/**
 *
 */
UCLASS()
class OVERLORD_API UCSRunState : public UCSCharacterState
{
	GENERATED_BODY()
public:
	UCSRunState();
protected:
	FVector RunTarget;
	float MoveSpeed;
	UPROPERTY()  UAnimMontage* StopRunMontage;
	bool bIsSprint;
	UPROPERTY() FTimerHandle SprintTimerHandle;
public:
	FORCEINLINE void SetRunTarget(FVector Target) { RunTarget = Target; };
	FORCEINLINE void SetMoveSpeed(float Speed) { MoveSpeed = Speed; };
	FORCEINLINE bool GetIsSprint() { return bIsSprint; };
	void SetIsSprint(bool IsSprint);
protected:
	void SetNotSprintDelay();
public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;
};
