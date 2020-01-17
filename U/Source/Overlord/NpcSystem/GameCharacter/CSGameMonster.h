// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSGameCharacter.h"
#include "CSGameMonster.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API UCSGameMonster : public UCSGameCharacter
{
	GENERATED_BODY()
public:
	UCSGameMonster();
public:
	virtual void Init(void* Info) override;
};
