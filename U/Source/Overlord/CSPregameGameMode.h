// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CSPregameGameMode.generated.h"

class FMessage;
/**
 *
 */
UCLASS()
class OVERLORD_API ACSPregameGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
