// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Protoc/move.pb.h"
#include "Protoc/test.pb.h"
#include "CSCharacterController.generated.h"


class ACSCharacter;
class USpringArmComponent;
class UCameraComponent;
/**
 *
 */
UCLASS()
class OVERLORD_API ACSCharacterController : public APlayerController
{
	GENERATED_BODY()
public:
	//virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY() ACSCharacter* ControlledCharacter;
	UPROPERTY() USpringArmComponent* SpringArm;
public:
	FORCEINLINE ACSCharacter* GetControlledCharacter()
	{
		if (GetPawn() == nullptr)return nullptr;
		ControlledCharacter = Cast<ACSCharacter>(GetPawn());
		return ControlledCharacter;
	}

	USpringArmComponent* GetSpringArm();
	UCameraComponent* GetCamera();
public:
	//触发自动摄像机
	UFUNCTION(BlueprintCallable, Category = Camera) void ActiveAutoAdjusetCameraSystem();
	//取消自动摄像机
	UFUNCTION(BlueprintCallable, Category = Camera) void DectiveAutoAdjusetCameraSystem();
protected:
	void  AutoAdjusetCamera(float DeltaTime);
	void  AutoAjusetCamera_Move(float DeltaTime);
	void  AutoAjusetCamera_Collision(float DeltaTime);
protected:

	float  GetSpringArmNoCollsionYaw(float Dis);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera) float WaitAutoAdjusetCamera_Move_Time = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera) float AutoAdjusetCamera_Move_Time = 2.f;
	float WaitAutoAdjusetCamera_Move_Timer = 0;
	float AutoAdjusetCamera_Move_Timer = 0;
	bool bActiveAtuoAdjusetCamera_Move;
	bool bWaitAdjusetCamera_Move;
	FRotator AutoAdjustCamera_Move_StartRot;
	FRotator AutoAdjustCamera_Move_EndRot;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera) float AutoAdjusetCamera_Collsion_TriggerDistance = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera) float AutoAdjusetCamera_Collsion_Time = 2.f;
	float AutoAdjusetCamera_Collsion_Timer;
	bool bActiveAtuoAdjusetCamera_Collsion;
	FRotator AutoAdjustCamera_Collsion_StartRot;
	FRotator AutoAdjusetCamera_Collsion_EndRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera) float AutoAdjusetCamera_NormalPitch = -30;
	bool bAtuoAdjusetCamera;
};
