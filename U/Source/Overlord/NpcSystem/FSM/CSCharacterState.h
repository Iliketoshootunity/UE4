// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSystem/FSM/FSMState.h"
#include "../Other/CSCharacterDefine.h"
#include "CSCharacterState.generated.h"


class ACSCharacter;
class UCSGameCharacter;
class UFSMMachine;

UCLASS()
class OVERLORD_API UCSCharacterState : public UFSMState
{
	GENERATED_BODY()
public:
	UCSCharacterState();
protected:
	ACSCharacter* Character;
	UCSGameCharacter* CharacterInfo;
	ECharaterType CharacterType;
public:
	virtual void Init(int32 StateId, int32 ToSlefTransition, UFSMMachine* InMachine, UObject* InOwner) override;
};
