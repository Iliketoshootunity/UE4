// Fill out your copyright notice in the Description page of Project Settings.


#include "CSRollMove_NS.h"
#include "NpcSystem/Character/CSCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BasicFunction.h"

UCSRollMove_NS::UCSRollMove_NS()
{
}



void UCSRollMove_NS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	AActor* A = MeshComp->GetOwner();
	ACSCharacter* Character = Cast<ACSCharacter>(A);
	if (Character)
	{
		UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
		FVector Forward = UKismetMathLibrary::GetForwardVector(FRotator(0, A->GetActorRotation().Yaw, 0));
		Movement->Velocity = Forward * Character->GetMaxRollSpeed();
	}
}
