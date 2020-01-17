// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSCharacterState.h"
#include "CSIdleState.generated.h"

class UAnimMontage;
/**
 * 
 */
UCLASS()
class OVERLORD_API UCSIdleState : public UCSCharacterState
{
	GENERATED_BODY()
public:
	UCSIdleState();
	UPROPERTY() UAnimMontage* FightIdle;
	UPROPERTY() UAnimMontage* FightIdle_ToIdle;
	bool bFightIdle;
	bool bSprintToIdle;
	float FightTime;
	float FightTimer;
public:
	FORCEINLINE bool IsFightIdle() { return bFightIdle; }
	FORCEINLINE void SetFightIdle(bool FightIdle) { bFightIdle = FightIdle; }
public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

};
