// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSCharacterDefine.generated.h"

UENUM(BlueprintType)
enum class ECharaterType :uint8
{
	MainPlayer = 0,
	Player,
	Monster,
	Npc
};