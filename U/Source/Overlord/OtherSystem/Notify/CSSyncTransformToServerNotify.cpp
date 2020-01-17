// Fill out your copyright notice in the Description page of Project Settings.


#include "CSSyncTransformToServerNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "NpcSystem/Character/CSCharacter.h"
#include "NpcSystem/GameCharacter/CSGameCharacter.h"
#include "NpcSystem/Other/CSCharacterDefine.h"
#include "Protoc/move.pb.h"
#include "SGameInstance.h"

void UCSSyncTransformToServerNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* A = MeshComp->GetOwner();
	ACSCharacter* Character = Cast<ACSCharacter>(A);
	if (Character)
	{
		UCSGameCharacter* CInfo = Character->GetCharacterInfo();
		if (CInfo)
		{
			if (CInfo->GetCharacterType() == ECharaterType::MainPlayer || CInfo->GetCharacterType() == ECharaterType::Monster)
			{
				int64 ID = Character->GetCharacterInfo()->GetID();
				float X = Character->GetActorLocation().X;
				float Y = Character->GetActorLocation().Y;
				float Z = Character->GetActorLocation().Z;
				float Yaw = Character->GetActorRotation().Yaw;
				moveV2::ReqSynPosition ReqPos;
				ReqPos.set_id(ID);
				ReqPos.set_x(X);
				ReqPos.set_y(Y);
				ReqPos.set_z(Z);
				ReqPos.set_dir(Yaw);
				g_pGameInstance->SendMessage(68012, &ReqPos);
			}
		}
	}
}
