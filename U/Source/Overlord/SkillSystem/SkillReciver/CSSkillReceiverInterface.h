// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interface.h"
#include "Engine/EngineTypes.h"
#include "../CSSkillData.h"
#include "CSSkillReceiverInterface.generated.h"


/**
 *
 */
UINTERFACE(Blueprintable)
class OVERLORD_API UCSSkillReceiverInterface : public UInterface
{
	GENERATED_BODY()
};

class ICSSkillReceiverInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Skill")
	void ReceiverSkill(FSkillReceiveInfo ReceiveInfo);
	virtual void ReceiverSkill_Implementation(FSkillReceiveInfo ReceiveInfo);
};
