// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubMapTable.h"
#include "SWorldDefine.h"

/**
 * 
 */
class OVERLORD_API FSubworld
{

private:
	int32                                               m_ID;
	int32                                               m_ParentID;
	FSubMapTableData*									m_pSubWorldInfo;
	float                                               m_fPixelWidth;                              //关卡像素点（X坐标的做大值）
	float                                               m_fPixelHeight;                             //关卡像素点（Y坐标的做大值）
	FVector2D                                           m_vMinPixelLoc;                             //关卡起始点最小坐标值（X,Y）
	FVector2D                                           m_vMaxPixelLoc;                             //关卡起始点最大坐标值（X,Y）
	int32                                               m_LinkSubWorldRegionID[ENineGridPos::Num];
	int32                                               m_LinkRegionID[ENineGridPos::Num];

public:
	FSubworld();
	FSubworld(int32 MapID, int32 SubWorldID);
	~FSubworld();

public:
	void        Init(int32 MapID, int32 SubWorldID);
	void        Shutdown();
	bool        Load();
	bool        UnLoad(bool IsOnlyHide = false);

	//是否在这个关卡上，param ： 玩家当前坐标
	bool        IsInLevel(FVector Location);

	//是否是相邻子场景
	bool        IsLinkSubWorld(int32 inSubWorldID);

public:
	FORCEINLINE int32      GetLinkSubWorldID(ENineGridPos::Type Pos)
	{
		return  m_LinkSubWorldRegionID[Pos];
	}

	FORCEINLINE int32       GetID()
	{
		return  m_ID;
	}

	FORCEINLINE FString    GetMapName()
	{
		if (!m_pSubWorldInfo) return ANSI_NONE;

		return m_pSubWorldInfo->MapName;
	}
};
