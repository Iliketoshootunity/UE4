// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateToTargetMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "BasicFunction.h"




URotateToTargetMovementComponent::URotateToTargetMovementComponent()
{
	bAutoRegisterPhysicsVolumeUpdates = false;
	bComponentShouldUpdatePhysicsVolume = false;


	bRotationInLocalSpace = true;
}

void URotateToTargetMovementComponent::StartRotate(FRotator EndRotation, float RotateRotate)
{
	AActor* Owner = GetOwner();
	FVector CurDir = UKismetMathLibrary::GetForwardVector(Owner->GetActorRotation());
	FVector EndDir = UKismetMathLibrary::GetForwardVector(EndRotation);
	RotatePivot = UKismetMathLibrary::Cross_VectorVector(CurDir, EndDir);
	RotatePivot.Normalize(0.0000001f);

	Temp.Roll = RotatePivot.X != 0 ? 1 : 0;
	if (RotatePivot.X < 0)Temp.Roll = -1;
	Temp.Yaw = RotatePivot.Z != 0 ? 1 : 0;
	if (RotatePivot.Z < 0)Temp.Yaw = -1;
	Temp.Pitch = RotatePivot.Y != 0 ? 1 : 0;
	if (RotatePivot.Y < 0)Temp.Pitch = -1;

	this->RotationRate = RotateRotate;
	this->EndRotation = EndRotation;
	bRoatating = true;
}

void URotateToTargetMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bRoatating)
	{
		AActor* Owner = GetOwner();
		FVector CurDir = UKismetMathLibrary::GetForwardVector(Owner->GetActorRotation());
		FVector EndDir = UKismetMathLibrary::GetForwardVector(EndRotation);
		float DotValue = UKismetMathLibrary::Dot_VectorVector(CurDir, EndDir);
		FString LogStr = FString::SanitizeFloat(DotValue);
		float Angle = UKismetMathLibrary::DegAcos(DotValue);
		if (Angle > 1)
		{
			// skip if we don't want component updated when not rendered or if updated component can't move
			if (ShouldSkipUpdate(DeltaTime))
			{
				return;
			}

			Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

			if (!IsValid(UpdatedComponent))
			{
				return;
			}
			// Compute new rotation
			const FQuat OldRotation = UpdatedComponent->GetComponentQuat();
			const FQuat TestRotate;

			const FQuat DeltaRotation = (Temp * RotationRate * DeltaTime).Quaternion();
			const FQuat NewRotation = bRotationInLocalSpace ? (OldRotation * DeltaRotation) : (DeltaRotation * OldRotation);

			// Compute new location
			FVector DeltaLocation = FVector::ZeroVector;
			if (!PivotTranslation.IsZero())
			{
				const FVector OldPivot = OldRotation.RotateVector(PivotTranslation);
				const FVector NewPivot = NewRotation.RotateVector(PivotTranslation);
				DeltaLocation = (OldPivot - NewPivot); // ConstrainDirectionToPlane() not necessary because it's done by MoveUpdatedComponent() below.
			}

			const bool bEnableCollision = false;
			MoveUpdatedComponent(DeltaLocation, NewRotation, bEnableCollision);
		}
		else
		{
			bRoatating = false;
		}
	}


}
