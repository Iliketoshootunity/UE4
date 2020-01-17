// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NpcSystem/GameCharacter/CSGamePlayer.h"
#include "CSGameMainPlayer.generated.h"

/**
 *
 */
UCLASS()
class OVERLORD_API UCSGameMainPlayer : public UCSGamePlayer
{
	GENERATED_BODY()
public:
	UCSGameMainPlayer();
public:
	virtual void Init(void* Info) override;

};
