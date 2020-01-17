// Fill out your copyright notice in the Description page of Project Settings.


#include "CSHitCheck_NS.h"
#include "NpcSystem/Character/CSCharacter.h"

UCSHitCheck_NS::UCSHitCheck_NS()
{

}

void UCSHitCheck_NS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* A = MeshComp->GetOwner();
	ACSCharacter* Character = Cast<ACSCharacter>(A);
	if (Character)
	{
		FHitCheckInfo HitInfo;
		HitInfo.ShapeType = ShapeType;
		HitInfo.LocaPosOffset = LocaPosOffset;
		HitInfo.ScaleX = ScaleX;
		HitInfo.ScaleY = ScaleY;
		HitInfo.ScaleZ = ScaleZ;
		HitInfo.HitPerformaceId = HitPerformaceId;
		HitInfo.HitFXRotate = HitFxRotate;
		Character->CreateAtkShape(HitInfo);
	}
}

void UCSHitCheck_NS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* A = MeshComp->GetOwner();
	ACSCharacter* Character = Cast<ACSCharacter>(A);
	if (Character)
	{
		Character->DestoryAtkShape();
	}
}
