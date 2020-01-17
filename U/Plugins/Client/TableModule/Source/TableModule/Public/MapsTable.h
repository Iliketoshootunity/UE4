// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"
#include "SubMapTable.h"

/************************************************************************
* Desc 	: ��ͼ����
* Author	: WangPeng
* Time		: [3/21/2019 wp]
************************************************************************/

typedef struct tagMapsTableData
{
    int32			ID;
    int32			ShowMapNameID;
    FString			MapName;
    FMapRect		MapRect;
    FRegionSize		RegionSize;             //width : Region�ĺ��������; height : Region�����������
    int32           WorldtoMeters;          //����ת������
    SubMapTable*	pSubMapTable;
    bool			HaveStreamMap;
	bool			IsNineGridLevel;		//�Ƿ��Զ�����Ź����ͼ
    FString			SubMapPath;
	int32			LoadingScreenID;

    tagMapsTableData()
    {
        pSubMapTable = NULL;
    }

}FMapsTableData;

class TABLEMODULE_API MapsTable : public STabBaseTable<MapsTable, FMapsTableData>
{
public:
	MapsTable();
	virtual ~MapsTable();

public:
    virtual bool				ReadTable(int32 nRow, int32& nCol) override;

    //�Ƿ����Submap
    bool						IsHaveSubMap(int32 MapID);

	//�Ƿ��ǾŹ����ͼ
	bool						IsNineGridMap(int32 MapID);

    //Submap��غ���
    const SubMapTable*			GetSubMapTable(int32 MapID);
    //��ȡSubMap����
    const FSubMapTableData*		GetSubMapData(int32 MapID, int32 SubMapIndex);
    //��ȡSubMap����
    FString						GetSubMapName(int32 MapID, int32 SubMapIndex);

    //���㵱ǰ�ӹؿ���ID
    uint64                      CalCurrentSubLevelID(int32 MapID, FVector Location);
};
         