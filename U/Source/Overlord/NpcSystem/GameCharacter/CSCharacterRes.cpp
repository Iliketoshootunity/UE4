// Fill out your copyright notice in the Description page of Project Settings.


#include "CSCharacterRes.h"
#include "NpcResTable.h"
#include "BaseSystem/GameUtil.h"
#include "SGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "NpcSystem/Character/CSCharacter.h"
#include "../Weapon/CSWeapon.h"

FCSCharacterRes::FCSCharacterRes()
{
	m_nCharIndex = INVALID_INDEX;
	m_PartMap.Empty();
	m_CharActorMap.Empty();
	m_pCharacterTypeData = nullptr;
	FCoreUObjectDelegates::GetPreGarbageCollectDelegate().AddRaw(this, &FCSCharacterRes::OnPreGarbageCollect);
}

FCSCharacterRes::~FCSCharacterRes()
{
	m_PartMap.Empty();
	m_CharActorMap.Empty();
	m_pCharacterTypeData = nullptr;
	FCoreUObjectDelegates::GetPreGarbageCollectDelegate().RemoveAll(this);
}

bool FCSCharacterRes::Init(FString CharacterBPName, FVector Pos, FRotator Rot)
{
	UClass* LoadOBJClass = UGameUtil::LoadClass<UClass>(CharacterBPName);
	if (!LoadOBJClass) return false;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.ObjectFlags = RF_Transactional;
	const auto Location = FVector(0);
	const auto Rotation = FTransform(FVector(0)).GetRotation().Rotator();
	ACSCharacter* pActor = g_pGameInstance->GetWorld()->SpawnActor<ACSCharacter>(LoadOBJClass, Pos, Rot, SpawnInfo);
	if (!pActor) return false;
	pActor->SetActorHiddenInGame(false);
	m_CharActorMap.Add(EActorUnit::eActorUnit, pActor);
	return true;

}

void FCSCharacterRes::CreateWeapon()
{
	UClass* LoadOBJClass = UGameUtil::LoadClass<UClass>("/Game/Blueprints/Player/BP_Weapon.BP_Weapon_C");
	if (!LoadOBJClass) return;
	Weapon = g_pGameInstance->GetWorld()->SpawnActor<ACSWeapon>(LoadOBJClass, FVector::ZeroVector, FRotator::ZeroRotator);
	ACSCharacter* Unit = GetActorUnit();
	if (Weapon.IsValid() && Unit)
	{
		Unit->SetWeapon(Weapon.Get());
		Unit->ShowWeapon();
		Weapon->SetCharacter(Unit);
	}
}

void FCSCharacterRes::DestoryUnit()
{
	if (GetActorUnit())
	{
		UGameUtil::DestroyActor(GetActorUnit());
	}
	if (Weapon != nullptr)
	{
		UGameUtil::DestroyActor(Weapon.Get());
	}
}

void FCSCharacterRes::SetPart(BODY_PART::Type ePartType, int32 nPartID)
{
	if (!IsVaildPart(ePartType))
		return;

	if (!IsVaildActor())
		return;

	if (m_PartMap.FindRef(ePartType) == nPartID)
		return;

	m_PartMap.Add(ePartType, nPartID);

	if (m_PartMap.FindRef(BODY_PART::BODY_PART_HAIR) > 0 && m_PartMap.FindRef(BODY_PART::BODY_PART_FACE) > 0 && m_PartMap.FindRef(BODY_PART::BODY_PART_SHOULDER) > 0 &&
		m_PartMap.FindRef(BODY_PART::BODY_PART_BODY) > 0 && m_PartMap.FindRef(BODY_PART::BODY_PART_BELT) > 0 && m_PartMap.FindRef(BODY_PART::BODY_PART_LEG) > 0)
	{
		RefreshPart();
	}
}

bool FCSCharacterRes::SetRideHorse(int32 nPartID)
{

	m_CharActorMap.Add(EActorUnit::eRideUnit, nullptr);
	return true;
}

void FCSCharacterRes::SetLocation(FVector Loc)
{
	ACSCharacter* pActor = m_CharActorMap.FindRef(EActorUnit::eActorUnit);
	if (!pActor)
	{
		return;
	}

	pActor->SetActorLocation(Loc);
}

void FCSCharacterRes::SetRotator(FRotator roc)
{
	ACSCharacter* pActor = m_CharActorMap.FindRef(EActorUnit::eActorUnit);
	if (!pActor)
	{
		return;
	}

	pActor->SetActorRotation(roc);
}

void FCSCharacterRes::SetScale(FVector scale)
{
	ACSCharacter* pActor = m_CharActorMap.FindRef(EActorUnit::eActorUnit);
	if (!pActor)
	{
		return;
	}

	pActor->SetActorScale3D(scale);
}

ACSCharacter* FCSCharacterRes::GetActorUnit()
{
	if (m_CharActorMap.Num() <= 0)
		return nullptr;
	ACSCharacter* Actor = m_CharActorMap[EActorUnit::eActorUnit];
	return Actor;
}

bool FCSCharacterRes::IsVaildPart(BODY_PART::Type ePartType)
{
	return ePartType > BODY_PART::BODY_PART_INVALID && ePartType < BODY_PART::BODY_PART_MAX;
}

bool FCSCharacterRes::IsVaildActor()
{
	if (!m_CharActorMap.Contains(EActorUnit::eActorUnit))
		return false;

	ACSCharacter* pActor = m_CharActorMap.FindRef(EActorUnit::eActorUnit);
	if (pActor == nullptr || !pActor->IsValidLowLevel())
		return false;

	return true;
}

void FCSCharacterRes::RefreshPart()
{
	if (m_pCharacterTypeData == nullptr)
		return;

	TArray<FString> SrcMeshFileList;
	for (TBodypartResMap::TIterator It(m_PartMap); It; ++It)
	{
		int32 ePartType = It.Key();
		int32 nPartID = It.Value();
		const FRolePartMeshData* pData = NpcResTable::Get()->GetBodyPartData(m_pCharacterTypeData->ID, ePartType, nPartID);
		if (!pData)
			continue;

		SrcMeshFileList.Add(pData->SkeletalMesh);

	}

	USkeletalMesh* pSkeletalMesh = UGameUtil::SkeletalMeshMerge(SrcMeshFileList, m_pCharacterTypeData->SkeletonName);
	if (!pSkeletalMesh)
		return;

	ACSCharacter* pActor = m_CharActorMap.FindRef(EActorUnit::eActorUnit);

	pActor->SetSkeletalMesh(pSkeletalMesh, FVector(m_pCharacterTypeData->Scale, m_pCharacterTypeData->Scale, m_pCharacterTypeData->Scale),
		(EAnimationMode::Type)m_pCharacterTypeData->AnimationMode,
		m_pCharacterTypeData->AnimInstanceName,
		m_pCharacterTypeData->Material);
}

void FCSCharacterRes::OnPreGarbageCollect()
{
	if (m_CharActorMap.Num() <= 0)
		return;

	for (TActorMap::TIterator It(m_CharActorMap); It; ++It)
	{
		//判断是否已经被GC，GC调后清除
		ACSCharacter* pActor = It.Value();
		if (pActor == nullptr || !pActor->IsValidLowLevel())
		{
			It.RemoveCurrent();
		}
	}
}
