// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../CSSkillData.h"
#include "CSAtkShapeBase.generated.h"

class ACSCharacter;
class UCSSkill;
class UCSComboClip;
class AActor;
class ICSSkillReceiverInterface;
UCLASS()
class OVERLORD_API ACSAtkShapeBase : public AActor
{
	GENERATED_BODY()
public:
	ACSAtkShapeBase();
protected:
	UPROPERTY()	float ComboId;
	UPROPERTY()	float AtkIndex;
	UPROPERTY(BlueprintReadWrite)	FVector LocaPosOffset;
	UPROPERTY(BlueprintReadWrite)	ACSCharacter* AtkOwner;
	UPROPERTY()	UCSSkill* Skill;
	UPROPERTY()	UCSComboClip* ComboClip;
	UPROPERTY()	TArray<AActor*> HasBeAtker;
	FHitCheckInfo HitChekInfo;
public:
	virtual void Init(ACSCharacter* InOwner, UCSSkill* InSkill, UCSComboClip* InClip,FHitCheckInfo InHitChekInfo);
	UFUNCTION(BlueprintCallable)
	virtual void AtkCheck();
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	TArray<FHitResult> BP_AtkCheck();
};
