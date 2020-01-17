// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameModeBase.h"
#include "SPreGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API ASPreGameMode : public ASGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void StartPlay() override;
	virtual void BeginDestroy() override;
};
