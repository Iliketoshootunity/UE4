// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSAtkShapeBase.h"
#include "CSAtkBox.generated.h"


UCLASS(Blueprintable)
class OVERLORD_API ACSAtkBox : public ACSAtkShapeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACSAtkBox();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	 float ScaleX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	 float ScaleY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	 float ScaleZ;
public:
	virtual void Init(ACSCharacter* InOwner,UCSSkill* InSkill,UCSComboClip* InClip,FHitCheckInfo InHitChekInfo) override;

};
