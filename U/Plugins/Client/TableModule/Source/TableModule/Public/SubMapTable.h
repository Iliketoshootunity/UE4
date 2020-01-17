// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

/**
 * ���ؿ������ļ���
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
    FString         MapShowName;                //��ʾ��
    FString         MapName;                    //��Դ��
    FMapRect        MapRect;                    //��ͼ��С
    FRegionSize     RegionSize;                 //�������
    float           WorldtoMeters;              //1uu = 1cm, scale 100,1uu = 1m
    int32           StreamMapX;                 //���ܿ�����λ��
    int32           StreamMapY;                 //���ؿ�����λ��
    bool            bDefaultInitialize;         //Ĭ�ϳ�ʼ��
    bool            bDefaultShow;               //Ĭ����ʾ
    bool            bHaveStreamMap;             //�Ƿ�����ӵ�ͼ
    FString         SubMapPath;                 //�ӵ�ͼ·��
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
