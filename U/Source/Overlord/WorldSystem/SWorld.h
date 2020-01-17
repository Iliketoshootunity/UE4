// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSubworld.h"
#include "MapsTable.h"
#include "SWorldDefine.h"
#include "BaseSystem/SAssetRef.h"

/**
 * 
 */
class OVERLORD_API FWorld
{

private:
	int32                                               m_MapID;                //地图ID
	UWorld*												m_pWorld;               //真实地图实体类
	FMapsTableData*										m_pMapInfo;
	int32												m_CurrentSubWorldID;	//当前子地图的索引
	bool                                                m_bLoadStatus;          //加载状态
	FSubworld											m_SubWorlds[ENineGridPos::Num];  //连接的子地图
	int32                                               m_nWorldWidth;          //场景的宽
	int32                                               m_nWorldHeight;         //场景的高

	FAssetRef											m_AssetRef;

public:
	uint64												m_dwCurrentTime;				//	当前帧

public:
	FWorld();
	~FWorld();

	void                                                Init();
	//关闭
	void                                                Shutdown();

	void                                                Tick(float fDeltaTime);

	//本实例是否有效
	bool												IsValid();

	//加载场景
	bool                                                LoadMap(int32 MapID);
	//加载地图包完成后调用
	void                                                DoLoadMap(FName MapName);

	bool                                                UnloadMap();

	//MapID 是否有效
	bool                                                IsValidMap(int32 MapID); 

	//the very beginning of LoadMap
	void                                                PreLoadMap(const FString& MapName);

	// the _successful_ end of LoadMap
	void                                                PostLoadMap(UWorld* mWorld);

public:
	FORCEINLINE FAssetRef*								GetAssetRef()
	{
		return &m_AssetRef;
	}

	FORCEINLINE int32									GetMapID()
	{
		return m_MapID;
	}
private:

	//根据玩家坐标计算当前子地图索引
	int32	                                            CalSubWorldID();

	//加载子场景
	bool                                                LoadSubMap();
	//卸载子场景
	bool                                                UnloadSubmap();

	FSubworld*											GetSubWorld(int32 SubWorldID);

	UWorld*												GetWorld();

public:

	FORCEINLINE FMapsTableData*							GetTableData()
	{
		return m_pMapInfo;
	}
};
