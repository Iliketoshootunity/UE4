// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorld.h"
#include "SGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "UISystem/SWidgetManager.h"
#include "BasicFunction.h"

FWorld::FWorld()
{
}

FWorld::~FWorld()
{
}

void FWorld::Init()
{
	m_MapID = INVALID_WORLD_INDEX;
	m_pWorld = NULL;
	m_pMapInfo = NULL;
	m_bLoadStatus = false;
	m_CurrentSubWorldID = INVALID_WORLD_INDEX;
	FPlatformMemory::Memzero(m_SubWorlds, sizeof(m_SubWorlds));
}

void FWorld::Shutdown()
{
	m_MapID = INVALID_WORLD_INDEX;
	m_pWorld = NULL;
	m_bLoadStatus = false;
	m_CurrentSubWorldID = INVALID_WORLD_INDEX;

	if (m_pMapInfo && (m_pMapInfo->HaveStreamMap && m_pMapInfo->pSubMapTable != NULL))
	{
		for (int32 pos = ENineGridPos::Center; pos < ENineGridPos::Num; ++pos)
		{
			m_SubWorlds[pos].UnLoad();
		}
	}

	m_pMapInfo = NULL;
	FPlatformMemory::Memzero(m_SubWorlds, sizeof(m_SubWorlds));
}

void FWorld::Tick(float fDeltaTime)
{
	if (m_MapID == INVALID_WORLD_INDEX)
		return;

	if (!m_pMapInfo || !m_pMapInfo->IsNineGridLevel)
		return;

	UnloadSubmap();
	LoadSubMap();
}

bool FWorld::IsValid()
{
	if (m_MapID == INVALID_WORLD_INDEX)
		return false;

	if (!m_pMapInfo)
		return false;

	return true;
}

bool FWorld::LoadMap(int32 MapID)
{
	if (!IsValidMap(MapID))
	{
		return false;
	}

	if (m_bLoadStatus)
	{
		return false;
	}

	//如果加载的地图相同，返回true，已经处理过
	if (m_MapID == MapID)
	{
		return true;
	}

	//获取Map.txt
	const FMapsTableData* pMapInfo = MapsTable::Get()->GetData(MapID);
	if (!pMapInfo) return false;


	//判断当前地图是否已经加载，引擎启动会加载默认地图
	UWorld* pCurWorld = GetWorld();

	if (pCurWorld == nullptr)
		return false;

	FString CurMapName = pCurWorld->GetName();

	//如果当前地图已经加载则不需要在加载
	if (pMapInfo->MapName.Contains(CurMapName))
		return false;


	
	//异步加载地图资源
	if (!m_AssetRef.Init(pMapInfo->MapName))
		return false;

	m_AssetRef.AssetAsyncLoad();

	m_pMapInfo = (FMapsTableData*)pMapInfo;
	m_MapID = MapID;

	return true;
}

void FWorld::DoLoadMap(FName MapName)
{

	UWorld* pCurWorld = GetWorld();

	if (pCurWorld == nullptr)
		return;

	FString CurMapName = pCurWorld->GetName();

	//如果当前地图已经加载则不需要在加载
	if (MapName.ToString().Contains(CurMapName))
		return;


	UGameplayStatics::OpenLevel((const UObject*)pCurWorld, MapName);

}

bool FWorld::UnloadMap()
{
	//卸载当前场景缓存资源包
	if (m_pMapInfo)
		FAssetRef::Unload(m_pMapInfo->MapName);

	Shutdown();
	return true;
}

bool FWorld::IsValidMap(int32 MapID)
{
	return MapsTable::Get()->IsValid(MapID);
}

void FWorld::PreLoadMap(const FString& MapName)
{
	if (!IsValid())
		return;
}

void FWorld::PostLoadMap(UWorld* mWorld)
{
	if (!IsValid())
		return;

	if (m_bLoadStatus)
		return;

	m_pWorld = mWorld;
	m_bLoadStatus = true;
	m_AssetRef.Reset();
	LoadSubMap();

	if(m_pMapInfo)
		FBasicFunction::Logs(TEXT("End load %s"), *m_pMapInfo->MapName);
}


int32 FWorld::CalSubWorldID()
{
	ACharacter* pCharacter = UGameplayStatics::GetPlayerCharacter(g_pGameInstance->GetWorld(), 0);
	if (!pCharacter)
		return INVALID_WORLD_INDEX;

	FVector Location = pCharacter->GetActorLocation();
	int32 SubWorldID = MapsTable::Get()->CalCurrentSubLevelID(m_MapID, Location);
	return SubWorldID;
}

bool FWorld::LoadSubMap()
{
	if (!m_bLoadStatus)
		return false;

	if (m_pMapInfo == NULL)
		return false;

	if (!m_pMapInfo->HaveStreamMap || m_pMapInfo->pSubMapTable == NULL)
		return false;

	int32 SubWorldID = CalSubWorldID();
	if (SubWorldID <= INVALID_WORLD_INDEX) return false;

	if (m_CurrentSubWorldID == SubWorldID)
		return false;

	//当前子场景已经存在
	m_SubWorlds[ENineGridPos::Center].Init(m_MapID, SubWorldID);
	for (int32 pos = ENineGridPos::Center; pos < ENineGridPos::Num; ++pos)
	{
		int32 LinkSubWorldID = m_SubWorlds[ENineGridPos::Center].GetLinkSubWorldID((ENineGridPos::Type)pos);
		m_SubWorlds[pos].Init(m_MapID, LinkSubWorldID);
		bool bResult = m_SubWorlds[pos].Load();
	}

	m_CurrentSubWorldID = SubWorldID;

	return true;
}

bool FWorld::UnloadSubmap()
{
	int32 NewSubWorldID = CalSubWorldID();
	if (NewSubWorldID <= INVALID_WORLD_INDEX) return false;

	if (m_CurrentSubWorldID <= INVALID_WORLD_INDEX)
		return false;

	if (m_CurrentSubWorldID == NewSubWorldID)
		return false;

	FSubworld NewSubWorld(m_MapID, NewSubWorldID);

	for (int32 pos = ENineGridPos::Center; pos < ENineGridPos::Num; ++pos)
	{
		int32 LinkSubWorldID = m_SubWorlds[ENineGridPos::Center].GetLinkSubWorldID((ENineGridPos::Type)pos);
		bool bResult = NewSubWorld.IsLinkSubWorld(LinkSubWorldID);
		if (!bResult)
		{
			FSubworld* pSubWorld = GetSubWorld(LinkSubWorldID);
			if (pSubWorld)
			{
				pSubWorld->UnLoad();
			}

		}
	}
	return true;
}

FSubworld* FWorld::GetSubWorld(int32 SubWorldID)
{
	for (int32 pos = ENineGridPos::Center; pos < ENineGridPos::Num; ++pos)
	{
		if (m_SubWorlds[pos].GetID() == SubWorldID)
			return &m_SubWorlds[pos];
	}

	return NULL;
}

UWorld* FWorld::GetWorld()
{
	if (!g_pGameInstance)
		return nullptr;

	return g_pGameInstance->GetWorld() ? g_pGameInstance->GetWorld() : GWorld ? GWorld : m_pWorld ? m_pWorld : nullptr;
}
