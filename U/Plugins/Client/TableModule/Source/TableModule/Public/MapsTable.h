// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"
#include "SubMapTable.h"

/************************************************************************
* Desc 	: 地图表类
* Author	: WangPeng
* Time		: [3/21/2019 wp]
************************************************************************/

typedef struct tagMapsTableData
{
    int32			ID;
    int32			ShowMapNameID;
    FString			MapName;
    FMapRect		MapRect;
    FRegionSize		RegionSize;             //width : Region的横向菱格数; height : Region的纵向菱格数
    int32           WorldtoMeters;          //场景转换比例
    SubMapTable*	pSubMapTable;
    bool			HaveStreamMap;
	bool			IsNineGridLevel;		//是否自动处理九宫格地图
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

    //是否存在Submap
    bool						IsHaveSubMap(int32 MapID);

	//是否是九宫格地图
	bool						IsNineGridMap(int32 MapID);

    //Submap相关函数
    const SubMapTable*			GetSubMapTable(int32 MapID);
    //获取SubMap数据
    const FSubMapTableData*		GetSubMapData(int32 MapID, int32 SubMapIndex);
    //获取SubMap名字
    FString						GetSubMapName(int32 MapID, int32 SubMapIndex);

    //计算当前子关卡的ID
    uint64                      CalCurrentSubLevelID(int32 MapID, FVector Location);
};
         