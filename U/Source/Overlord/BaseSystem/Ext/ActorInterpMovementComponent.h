// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "ActorInterpMovementComponent.generated.h"

/**
 * 角色插值方式
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class OVERLORD_API UActorInterpMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:
	UActorInterpMovementComponent(const FObjectInitializer& ObjectInitializer);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
	virtual void StopMovementImmediately() override;
	UFUNCTION(BlueprintCallable, Category="Game|Components|ActorInterpMovementComponent")
	void StartMovement(float EndYaw,FVector TargetPostion, float Time, bool bRelative = false,float StartTimer = 0);
	UFUNCTION(BlueprintCallable, Category = "Game|Components|ActorInterpMovementComponent")
	bool IsPlay();
	//////////////////////////////////////////////////////////////////////////
	// Rotate to Yaw
public:
	UFUNCTION(BlueprintCallable, Category="Game|Components|ActorInterpMovementComponent")
	void StartRotateToTagetYaw(float StartYaw, float EndYaw, float Time, float StartTimer = 0);
	void StartRotateToTagetYaw(float EndYaw, float Time, float StartTimer = 0);
	UFUNCTION(BlueprintCallable, Category="Game|Components|ActorInterpMovementComponent")
	void StopRotateToTargetYaw();
protected:
	void UpdateRotateToTargetYaw(float DeltaTime);
protected:
	bool bRotateToYaw;
	float RotateToYawTimer;
	float RotateToYawTime;
	FQuat RotartToTagetYaw_Start;
	FQuat RotartToTagetYaw_End;

	//////////////////////////////////////////////////////////////////////////
	// Move to Target
public:
	UFUNCTION(BlueprintCallable, Category="Game|Components|ActorInterpMovementComponent")
	void StartMoveToTarget(FVector TargetPostion, float Time, bool bRelative = false);
	UFUNCTION(BlueprintCallable, Category="Game|Components|ActorInterpMovementComponent")
	void StopMoveToTarget();
protected:
	void UpdateMoveToTarget(float DeltaTime);
protected:
	bool bMoveToTarget;
	float MoveToTargetTimer;
	float MoveToTargetTime;
	FVector MoveSpeed;

};
