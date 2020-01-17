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
	int32                                               m_MapID;                //��ͼID
	UWorld*												m_pWorld;               //��ʵ��ͼʵ����
	FMapsTableData*										m_pMapInfo;
	int32												m_CurrentSubWorldID;	//��ǰ�ӵ�ͼ������
	bool                                                m_bLoadStatus;          //����״̬
	FSubworld											m_SubWorlds[ENineGridPos::Num];  //���ӵ��ӵ�ͼ
	int32                                               m_nWorldWidth;          //�����Ŀ�
	int32                                               m_nWorldHeight;         //�����ĸ�

	FAssetRef											m_AssetRef;

public:
	uint64												m_dwCurrentTime;				//	��ǰ֡

public:
	FWorld();
	~FWorld();

	void                                                Init();
	//�ر�
	void                                                Shutdown();

	void                                                Tick(float fDeltaTime);

	//��ʵ���Ƿ���Ч
	bool												IsValid();

	//���س���
	bool                                                LoadMap(int32 MapID);
	//���ص�ͼ����ɺ����
	void                                                DoLoadMap(FName MapName);

	bool                                                UnloadMap();

	//MapID �Ƿ���Ч
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

	//�������������㵱ǰ�ӵ�ͼ����
	int32	                                            CalSubWorldID();

	//�����ӳ���
	bool                                                LoadSubMap();
	//ж���ӳ���
	bool                                                UnloadSubmap();

	FSubworld*											GetSubWorld(int32 SubWorldID);

	UWorld*												GetWorld();

public:

	FORCEINLINE FMapsTableData*							GetTableData()
	{
		return m_pMapInfo;
	}
};
