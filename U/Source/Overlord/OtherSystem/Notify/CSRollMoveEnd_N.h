// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NpcSystem/Character/CSCharacter.h"
#include "CSRollMoveEnd_N.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API UCSRollMoveEnd_N : public UAnimNotify
{
	GENERATED_BODY()
public:
	UCSRollMoveEnd_N();
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
