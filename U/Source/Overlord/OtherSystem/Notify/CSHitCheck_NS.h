// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SkillSystem/CSSkillData.h"
#include "CSHitCheck_NS.generated.h"



/**
 *
 */
UCLASS()
class OVERLORD_API UCSHitCheck_NS : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UCSHitCheck_NS();
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	EAtkShapeType ShapeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FVector LocaPosOffset = FVector(200, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	float ScaleX = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	float ScaleY = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	float ScaleZ = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	float HitPerformaceId = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FRotator HitFxRotate ;
};
