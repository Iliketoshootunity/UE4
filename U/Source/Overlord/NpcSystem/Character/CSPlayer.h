// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protoc/fight.pb.h"
#include "Protoc/move.pb.h"
#include "CSCharacter.h"
#include "CSPlayer.generated.h"

/**
 *
 */
UCLASS()
class OVERLORD_API ACSPlayer : public ACSCharacter
{
	GENERATED_BODY()
public:
	ACSPlayer();
public:
	//////////////////////////////////////////////////////////////////////////
	// 输入操作相关
	UFUNCTION(BlueprintCallable, Category = Input) void OnRotateToTagrget(float RotateTime);
	void CoverOperation(float NewRockerX, float NewRockerY, float NewMaxRotate, float NewSourceYaw, float NewControllerYaw, float NewTargetYaw);	//*覆盖操作信息*/

	float CacheMaxRotate;
	float CacheSourceYaw;
	float CacheTargetYaw;
	float CacheRockerX;
	float CacheRockerY;
	float CacheControllerYaw;

};
