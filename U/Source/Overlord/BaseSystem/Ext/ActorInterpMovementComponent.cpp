// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorInterpMovementComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UActorInterpMovementComponent::UActorInterpMovementComponent(const FObjectInitializer& ObjectInitialize)
	: Super(ObjectInitialize)
{
	bAutoRegisterUpdatedComponent = true;
	bAutoRegisterPhysicsVolumeUpdates = false;
	bComponentShouldUpdatePhysicsVolume = false;
}

void UActorInterpMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UActorInterpMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UpdateRotateToTargetYaw(DeltaTime);
	UpdateMoveToTarget(DeltaTime);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UActorInterpMovementComponent::StopMovementImmediately()
{
	StopMoveToTarget();
	StopRotateToTargetYaw();
	Super::StopMovementImmediately();
}


void UActorInterpMovementComponent::StartMovement(float EndYaw, FVector TargetPostion, float Time, bool bRelative /*= false*/, float StartTimer /*= 0*/)
{
	StartRotateToTagetYaw(GetOwner()->GetActorRotation().Yaw, EndYaw, Time, StartTimer);
	StartMoveToTarget(TargetPostion, Time, bRelative);
}

bool UActorInterpMovementComponent::IsPlay()
{
	if (bRotateToYaw || bMoveToTarget)
	{
		return true;
	}
	return false;
}

void UActorInterpMovementComponent::StartRotateToTagetYaw(float StartYaw, float EndYaw, float Time, float StartTimer /*= 0*/)
{
	float ActorPitch = GetOwner()->GetActorRotation().Pitch;
	float ActorRoll = GetOwner()->GetActorRotation().Roll;
	FRotator StartRotation = FRotator(ActorPitch, StartYaw, ActorRoll);
	FRotator EndRotation = FRotator(ActorPitch, EndYaw, ActorRoll);
	RotartToTagetYaw_Start = FQuat::MakeFromEuler(FVector(StartRotation.Roll, StartRotation.Pitch, StartRotation.Yaw));
	RotartToTagetYaw_End = FQuat::MakeFromEuler(FVector(EndRotation.Roll, EndRotation.Pitch, EndRotation.Yaw));
	RotateToYawTimer = StartTimer;
	if (Time < 0)
	{
		RotateToYawTime = 0.00001;
		UpdateRotateToTargetYaw(10);
		return;
	}
	RotateToYawTime = Time;
	bRotateToYaw = true;
}

void UActorInterpMovementComponent::StartRotateToTagetYaw(float EndYaw, float Time, float StartTimer /*= 0*/)
{
	float ActorYaw = GetOwner()->GetActorRotation().Yaw;
	StartRotateToTagetYaw(ActorYaw, EndYaw, Time, StartTimer);
}

void UActorInterpMovementComponent::StopRotateToTargetYaw()
{
	bRotateToYaw = false;
}

void UActorInterpMovementComponent::UpdateRotateToTargetYaw(float DeltaTime)
{
	if (bRotateToYaw)
	{
		RotateToYawTimer += DeltaTime;
		float process = RotateToYawTimer / RotateToYawTime;
		if (process > 1)process = 1;
		FQuat QuatTemp = FQuat::Slerp(RotartToTagetYaw_Start, RotartToTagetYaw_End, process);
		FRotator Rot = QuatTemp.Rotator();
		GetOwner()->SetActorRotation(Rot);
		if (process == 1) StopRotateToTargetYaw();
	}
}

void UActorInterpMovementComponent::StartMoveToTarget(FVector TargetPostion, float Time, bool bRelative /*= false*/)
{
	if (bRelative)
	{
		FVector NewTargetPostion= UKismetMathLibrary::GreaterGreater_VectorRotator(TargetPostion, GetOwner()->GetActorRotation());
		TargetPostion = NewTargetPostion;
	}
	if (Time < 0)
	{
		GetOwner()->SetActorLocation(TargetPostion);
		return;
	}
	bMoveToTarget = true;
	FVector StartPosition = GetOwner()->GetActorLocation();
	FVector PositionDif = TargetPostion - StartPosition;
	MoveToTargetTimer = 0;
	MoveToTargetTime = Time;
	MoveSpeed = PositionDif / MoveToTargetTime;
}

void UActorInterpMovementComponent::StopMoveToTarget()
{
	bMoveToTarget = false;
}

void UActorInterpMovementComponent::UpdateMoveToTarget(float DeltaTime)
{
	if (bMoveToTarget)
	{
		float TempTimer = MoveToTargetTimer;
		float TempDeltaTime = DeltaTime;
		MoveToTargetTimer += DeltaTime;
		if (MoveToTargetTimer > MoveToTargetTime)
		{
			TempDeltaTime = MoveToTargetTime - TempTimer;
			bMoveToTarget = false;
		}
		FVector PositionDelta = MoveSpeed * TempDeltaTime;
		MoveUpdatedComponent(PositionDelta, GetOwner()->GetActorRotation(), true);
	}
}

