// Fill out your copyright notice in the Description page of Project Settings.


#include "CSRollMoveEnd_N.h"
#include "BaseSystem/FSM/FSMMachine.h"

UCSRollMoveEnd_N::UCSRollMoveEnd_N()
{
	
}

void UCSRollMoveEnd_N::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* A = MeshComp->GetOwner();
	ACSCharacter* Character = Cast<ACSCharacter>(A);
	if (Character)
	{
		Character->ToIdle();
		Character->SetCanRoll(true);
	}
}
