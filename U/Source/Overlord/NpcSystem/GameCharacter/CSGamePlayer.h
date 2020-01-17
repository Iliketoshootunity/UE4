// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSGameCharacter.h"
#include "CSGamePlayer.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API UCSGamePlayer : public UCSGameCharacter
{
	GENERATED_BODY()
public:
	UCSGamePlayer();
public:
	virtual void Init(void* Info) override;
};
