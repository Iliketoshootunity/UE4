// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSCharacter.h"
#include "CSMonster.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API ACSMonster : public ACSCharacter
{
	GENERATED_BODY()
public:
	ACSMonster();
//public:
//	UFUNCTION(BlueprintImplementableEvent, Category = Move) FVector CalNextTarget(FVector OriginPos);
};
