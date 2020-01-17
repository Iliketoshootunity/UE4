// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UCSComboEnd_N.generated.h"

/**
 *
 */
UCLASS()
class OVERLORD_API UUCSComboEnd_N : public UAnimNotify
{
	GENERATED_BODY()
public:
	UUCSComboEnd_N();
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
