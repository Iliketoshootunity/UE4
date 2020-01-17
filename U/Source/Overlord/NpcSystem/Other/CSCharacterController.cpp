// Fill out your copyright notice in the Description page of Project Settings.


#include "CSCharacterController.h"
#include "BaseSystem/Ext/MathfExtLibrary.h"
#include "SGameInstance.h"
#include "GameFramework/Pawn.h"
#include "../Character/CSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"




void ACSCharacterController::Tick(float DeltaTime)
{
	AutoAdjusetCamera(DeltaTime);
}



USpringArmComponent* ACSCharacterController::GetSpringArm()
{
	if (GetControlledCharacter() == nullptr)return nullptr;
	return GetControlledCharacter()->GetCameraBoom();
}

UCameraComponent* ACSCharacterController::GetCamera()
{
	if (GetControlledCharacter() == nullptr)return nullptr;
	return GetControlledCharacter()->GetCamera();
}

void ACSCharacterController::ActiveAutoAdjusetCameraSystem()
{
	bAtuoAdjusetCamera = true;

	bActiveAtuoAdjusetCamera_Move = false;
	bWaitAdjusetCamera_Move = false;
	bActiveAtuoAdjusetCamera_Collsion = false;
}

void ACSCharacterController::DectiveAutoAdjusetCameraSystem()
{
	bAtuoAdjusetCamera = false;
}

void ACSCharacterController::AutoAdjusetCamera(float DeltaTime)
{
	if (bAtuoAdjusetCamera)
	{
		AutoAjusetCamera_Move(DeltaTime);
		AutoAjusetCamera_Collision(DeltaTime);
	}

}

void ACSCharacterController::AutoAjusetCamera_Move(float DeltaTime)
{
	if (GetPawn() == nullptr)return;
	if (bActiveAtuoAdjusetCamera_Collsion)return; //碰撞矫正摄像机的优先级高于移动过程中矫正
	if (bActiveAtuoAdjusetCamera_Move && !bWaitAdjusetCamera_Move)
	{
		WaitAutoAdjusetCamera_Move_Timer += DeltaTime;
		if (WaitAutoAdjusetCamera_Move_Timer > WaitAutoAdjusetCamera_Move_Time)
		{
			bWaitAdjusetCamera_Move = true;
		}
		AutoAdjustCamera_Move_StartRot = GetControlRotation();
		AutoAdjustCamera_Move_EndRot = FRotator(AutoAdjusetCamera_NormalPitch, GetControlRotation().Yaw, GetControlRotation().Roll);
	}
	else if (bActiveAtuoAdjusetCamera_Move && bWaitAdjusetCamera_Move)
	{
		AutoAdjusetCamera_Move_Timer += DeltaTime;
		float Process = AutoAdjusetCamera_Move_Timer / AutoAdjusetCamera_Move_Time;
		if (Process >= 1)
		{
			Process = 1;
		}
		FQuat StartQuat = FQuat::MakeFromEuler(FVector(AutoAdjustCamera_Move_StartRot.Roll, AutoAdjustCamera_Move_StartRot.Pitch, AutoAdjustCamera_Move_StartRot.Yaw));
		FQuat EndQuat = FQuat::MakeFromEuler(FVector(AutoAdjustCamera_Move_EndRot.Roll, AutoAdjustCamera_Move_EndRot.Pitch, AutoAdjustCamera_Move_EndRot.Yaw));
		FQuat Quat = FQuat::Slerp(StartQuat, EndQuat, Process);
		SetControlRotation(Quat.Rotator());
		if (Process == 1)
		{
			bActiveAtuoAdjusetCamera_Move = false;
			bWaitAdjusetCamera_Move = false;
		}
	}
	else
	{
		//在发生位移时自动矫正镜头
		if (GetPawn()->GetVelocity() != FVector::ZeroVector)
		{
			bActiveAtuoAdjusetCamera_Move = true;
			bWaitAdjusetCamera_Move = false;
			AutoAdjusetCamera_Move_Timer = 0;
			WaitAutoAdjusetCamera_Move_Timer = 0;
		}
	}
}

void ACSCharacterController::AutoAjusetCamera_Collision(float DeltaTime)
{
	if (GetControlledCharacter() == nullptr || GetCamera() == nullptr)return;
	float Distance = FVector::Distance(GetControlledCharacter()->GetActorLocation(), GetCamera()->GetComponentLocation());
	if (!bActiveAtuoAdjusetCamera_Collsion)
	{
		if (GetSpringArm()->bIsCameraFixed && Distance < AutoAdjusetCamera_Collsion_TriggerDistance)
		{
			bActiveAtuoAdjusetCamera_Collsion = true;
			if (bActiveAtuoAdjusetCamera_Move)
			{
				bActiveAtuoAdjusetCamera_Move = false;
				bWaitAdjusetCamera_Move = false;
			}
			FRotator Rot = GetControlRotation();
			GetSpringArm()->TargetArmLength = 600;
			float EndYaw = GetSpringArmNoCollsionYaw(600);
			AutoAdjustCamera_Collsion_StartRot = Rot;
			AutoAdjusetCamera_Collsion_EndRot = FRotator(AutoAdjusetCamera_NormalPitch, EndYaw, Rot.Roll);
			AutoAdjusetCamera_Collsion_Timer = 0;
		}
	}
	else
	{
		AutoAdjusetCamera_Collsion_Timer += DeltaTime;
		float Process = AutoAdjusetCamera_Collsion_Timer / AutoAdjusetCamera_Collsion_Time;
		if (Process >= 1)
		{
			Process = 1;
		}
		FQuat StartQuat = FQuat::MakeFromEuler(FVector(AutoAdjustCamera_Collsion_StartRot.Roll, AutoAdjustCamera_Collsion_StartRot.Pitch, AutoAdjustCamera_Collsion_StartRot.Yaw));
		FQuat EndQuat = FQuat::MakeFromEuler(FVector(AutoAdjusetCamera_Collsion_EndRot.Roll, AutoAdjusetCamera_Collsion_EndRot.Pitch, AutoAdjusetCamera_Collsion_EndRot.Yaw));
		FQuat Quat = FQuat::Slerp(StartQuat, EndQuat, Process);
		SetControlRotation(Quat.Rotator());
		if (Process == 1)
		{
			bActiveAtuoAdjusetCamera_Collsion = false;
		}
	}
}

float ACSCharacterController::GetSpringArmNoCollsionYaw(float Dis)
{
	if (GetWorld() == nullptr)
	{
		return 0;
	}
	if (GetSpringArm() == nullptr)
	{
		return 0;
	}
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, GetSpringArm()->GetOwner());
	int Count = 8;
	float AngleOne = 360.f / Count;
	for (int32 i = 0; i < Count; i++)
	{
		FRotator Rot = FRotator(AutoAdjusetCamera_NormalPitch, GetControlRotation().Yaw + AngleOne * i, 0);
		FVector EndPos = -UKismetMathLibrary::GetForwardVector(Rot) * Dis + GetSpringArm()->PreviousArmOrigin;
		FHitResult Result;
		GetWorld()->SweepSingleByChannel(Result, GetSpringArm()->PreviousArmOrigin, EndPos, FQuat::Identity, GetSpringArm()->ProbeChannel, FCollisionShape::MakeSphere(GetSpringArm()->ProbeSize), QueryParams);
		if (Result.Actor == nullptr)
		{
			return  GetControlRotation().Yaw + AngleOne * i;
		}
	}
	return 0;
}

