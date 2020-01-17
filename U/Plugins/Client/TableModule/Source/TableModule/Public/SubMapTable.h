// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

/**
 * 流关卡配置文件类
 */
typedef struct tagMapRect
{
    int32 left;
    int32 top;
    int32 right;
    int32 bottom;
}FMapRect;

typedef struct tagRegionSize
{
    int32 width;
    int32 height;
}FRegionSize;

typedef struct tagSubMapTableData
{
    FString         MapShowName;                //显示名
    FString         MapName;                    //资源名
    FMapRect        MapRect;                    //地图大小
    FRegionSize     RegionSize;                 //区域宽，高
    float           WorldtoMeters;              //1uu = 1cm, scale 100,1uu = 1m
    int32           StreamMapX;                 //流管卡的行位置
    int32           StreamMapY;                 //流关卡的列位置
    bool            bDefaultInitialize;         //默认初始化
    bool            bDefaultShow;               //默认显示
    bool            bHaveStreamMap;             //是否存在子地图
    FString         SubMapPath;                 //子地图路径
}FSubMapTableData;

class TABLEMODULE_API SubMapTable : public MTabBaseTable<SubMapTable, FSubMapTableData>
{
public:
	SubMapTable();
    virtual ~SubMapTable();

public:
    virtual bool                ReadTable(int32 nRow, int32& nCol) override;

    void                        GetDefaultInitMapRegionID(TArray<int32>& arr);

    int32                       GetDefaultShowMapRegionID();

    void                        GetDefaultMapRegionID(TArray<int32>& DefaultInitIDs, TArray<int32>& DefaultShowIDs);

    bool                        IsInLevel(int32 ID, FVector Location);

    bool                        IsInLevel(const FSubMapTableData* Info, const FVector Location);
};
