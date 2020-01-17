// Fill out your copyright notice in the Description page of Project Settings.


#include "UCSComboEnd_N.h"
#include "NpcSystem/Character/CSCharacter.h"

UUCSComboEnd_N::UUCSComboEnd_N()
{

}

void UUCSComboEnd_N::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* A = MeshComp->GetOwner();
	ACSCharacter* Character = Cast<ACSCharacter>(A);
	if (Character)
	{
		Character->ReleaseSkillData();
		Character->ToIdle();
	}
}
