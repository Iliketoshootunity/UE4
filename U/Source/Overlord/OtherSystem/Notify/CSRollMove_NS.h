// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CSRollMove_NS.generated.h"

/**
 *
 */
UCLASS()
class OVERLORD_API UCSRollMove_NS : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UCSRollMove_NS();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
	float DataTest;
public:
	//virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
};
