// Fill out your copyright notice in the Description page of Project Settings.


#include "SSubworld.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "SGameInstance.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "MapsTable.h"
#include "BasicFunction.h"

FSubworld::FSubworld()
{
	m_ID = INVALID_WORLD_INDEX;
	m_ParentID = INVALID_WORLD_INDEX;
	m_pSubWorldInfo = NULL;
	m_fPixelWidth = 0.f;
	m_fPixelHeight = 0.f;
	m_vMinPixelLoc = FVector2D::ZeroVector;
	m_vMaxPixelLoc = FVector2D::ZeroVector;
	FPlatformMemory::Memzero(m_LinkSubWorldRegionID, sizeof(m_LinkSubWorldRegionID));
}

FSubworld::FSubworld(int32 MapID, int32 SubWorldID)
{
	m_ID = INVALID_WORLD_INDEX;
	m_ParentID = INVALID_WORLD_INDEX;
	m_pSubWorldInfo = NULL;
	m_fPixelWidth = 0.f;
	m_fPixelHeight = 0.f;
	m_vMinPixelLoc = FVector2D::ZeroVector;
	m_vMaxPixelLoc = FVector2D::ZeroVector;
	FPlatformMemory::Memzero(m_LinkSubWorldRegionID, sizeof(m_LinkSubWorldRegionID));

	Init(MapID, SubWorldID);
}

FSubworld::~FSubworld()
{
	m_ID = INVALID_WORLD_INDEX;
	m_ParentID = INVALID_WORLD_INDEX;
	m_pSubWorldInfo = NULL;
	m_fPixelWidth = 0.f;
	m_fPixelHeight = 0.f;
	m_vMinPixelLoc = FVector2D::ZeroVector;
	m_vMaxPixelLoc = FVector2D::ZeroVector;
	FPlatformMemory::Memzero(m_LinkSubWorldRegionID, sizeof(m_LinkSubWorldRegionID));
}

void FSubworld::Init(int32 MapID, int32 SubWorldID)
{
	const FSubMapTableData* pSubWorldInfo = MapsTable::Get()->GetSubMapData(MapID, SubWorldID);
	if (pSubWorldInfo == NULL) return;

	m_ID = SubWorldID;
	m_ParentID = MapID;
	m_pSubWorldInfo = (FSubMapTableData*)pSubWorldInfo;

	m_fPixelWidth = (pSubWorldInfo->MapRect.right - pSubWorldInfo->MapRect.left) * pSubWorldInfo->WorldtoMeters;
	m_fPixelHeight = (pSubWorldInfo->MapRect.bottom - pSubWorldInfo->MapRect.top) * pSubWorldInfo->WorldtoMeters;
	m_vMinPixelLoc = FVector2D(pSubWorldInfo->StreamMapX * m_fPixelWidth - m_fPixelWidth, pSubWorldInfo->StreamMapY * m_fPixelHeight - m_fPixelHeight);
	m_vMaxPixelLoc = FVector2D(pSubWorldInfo->StreamMapX * m_fPixelWidth, pSubWorldInfo->StreamMapY * m_fPixelHeight);

	m_LinkSubWorldRegionID[ENineGridPos::Center] = SubWorldID;
	m_LinkSubWorldRegionID[ENineGridPos::Left] = FBasicFunction::MakeU32(pSubWorldInfo->StreamMapX - 1, pSubWorldInfo->StreamMapY);
	m_LinkSubWorldRegionID[ENineGridPos::LeftAndTop] = FBasicFunction::MakeU32(pSubWorldInfo->StreamMapX - 1, pSubWorldInfo->StreamMapY - 1);
	m_LinkSubWorldRegionID[ENineGridPos::Top] = FBasicFunction::MakeU32(pSubWorldInfo->StreamMapX, pSubWorldInfo->StreamMapY - 1);
	m_LinkSubWorldRegionID[ENineGridPos::RightAndTop] = FBasicFunction::MakeU32(pSubWorldInfo->StreamMapX + 1, pSubWorldInfo->StreamMapY - 1);
	m_LinkSubWorldRegionID[ENineGridPos::Right] = FBasicFunction::MakeU32(pSubWorldInfo->StreamMapX + 1, pSubWorldInfo->StreamMapY);
	m_LinkSubWorldRegionID[ENineGridPos::RightAndBottom] = FBasicFunction::MakeU32(pSubWorldInfo->StreamMapX + 1, pSubWorldInfo->StreamMapY + 1);
	m_LinkSubWorldRegionID[ENineGridPos::Bottom] = FBasicFunction::MakeU32(pSubWorldInfo->StreamMapX, pSubWorldInfo->StreamMapY + 1);
	m_LinkSubWorldRegionID[ENineGridPos::LeftAndBottom] = FBasicFunction::MakeU32(pSubWorldInfo->StreamMapX - 1, pSubWorldInfo->StreamMapY + 1);

	if (pSubWorldInfo->bDefaultInitialize)
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.UUID = m_ID;
		UGameplayStatics::LoadStreamLevel(g_pGameInstance->GetWorld(), FName(*m_pSubWorldInfo->MapName), pSubWorldInfo->bDefaultShow, false, LatentInfo);
	}
}

void FSubworld::Shutdown()
{
	m_ID = INVALID_WORLD_INDEX;
	m_ParentID = INVALID_WORLD_INDEX;
	m_pSubWorldInfo = NULL;
	m_fPixelWidth = 0.f;
	m_fPixelHeight = 0.f;
	m_vMinPixelLoc = FVector2D::ZeroVector;
	m_vMaxPixelLoc = FVector2D::ZeroVector;
	FPlatformMemory::Memzero(m_LinkSubWorldRegionID, sizeof(m_LinkSubWorldRegionID));
}

bool FSubworld::Load()
{
	if (m_pSubWorldInfo == NULL) return false;

	if (g_pGameInstance->GetWorld() != nullptr)
	{
		//判断是否已经加载过
		ULevelStreaming* pLevelStreaming = UGameplayStatics::GetStreamingLevel(g_pGameInstance->GetWorld(), FName(*m_pSubWorldInfo->MapName));
		if (pLevelStreaming->IsLevelLoaded())
		{
			//如果已经加载做，判断是否是可见的
			if (!pLevelStreaming->IsLevelVisible())
			{
				pLevelStreaming->SetShouldBeVisible(true);
			}
			return true;
		}

		//没有加载过
		FLatentActionInfo LatentInfo;
		LatentInfo.UUID = m_ID;
		UGameplayStatics::LoadStreamLevel(g_pGameInstance->GetWorld(), FName(*m_pSubWorldInfo->MapName), true, false, LatentInfo);

		return true;
	}

	Shutdown();
	return false;
}

bool FSubworld::UnLoad(bool IsOnlyHide /*= false*/)
{
	if (g_pGameInstance->GetWorld() != nullptr && m_pSubWorldInfo != NULL)
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.UUID = m_ID;

		if (IsOnlyHide)
		{
			ULevelStreaming* pLevelStreaming = UGameplayStatics::GetStreamingLevel(g_pGameInstance->GetWorld(), FName(*m_pSubWorldInfo->MapName));
			pLevelStreaming->SetShouldBeVisible(false);
		}
		else
		{
			UGameplayStatics::UnloadStreamLevel(g_pGameInstance->GetWorld(), FName(*m_pSubWorldInfo->MapName), LatentInfo, false);

			Shutdown();
		}

		//FTimerHandle time_handler;
		//pHGameInstance->GetWorld()->GetTimerManager().SetTimer(time_handler, FTimerDelegate::CreateStatic(this, &SSubWorld::DelayUnLoad), 0.3f, false);
	}

	return true;
}

bool FSubworld::IsInLevel(FVector Location)
{
	if (m_ID == INVALID_WORLD_INDEX)
		return false;

	if (Location.X >= m_vMinPixelLoc.X && Location.X < m_vMaxPixelLoc.X && Location.Y >= m_vMinPixelLoc.Y && Location.Y < m_vMaxPixelLoc.Y)
	{
		return true;
	}

	return false;
}

bool FSubworld::IsLinkSubWorld(int32 inSubWorldID)
{
	for (int32 pos = ENineGridPos::Center; pos < ENineGridPos::Num; ++pos)
	{
		if (m_LinkSubWorldRegionID[pos] == inSubWorldID)
		{
			return true;
		}
	}

	return false;
}