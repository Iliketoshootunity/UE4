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
	float                                               m_fPixelWidth;                              //�ؿ����ص㣨X���������ֵ��
	float                                               m_fPixelHeight;                             //�ؿ����ص㣨Y���������ֵ��
	FVector2D                                           m_vMinPixelLoc;                             //�ؿ���ʼ����С����ֵ��X,Y��
	FVector2D                                           m_vMaxPixelLoc;                             //�ؿ���ʼ���������ֵ��X,Y��
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

	//�Ƿ�������ؿ��ϣ�param �� ��ҵ�ǰ����
	bool        IsInLevel(FVector Location);

	//�Ƿ��������ӳ���
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
