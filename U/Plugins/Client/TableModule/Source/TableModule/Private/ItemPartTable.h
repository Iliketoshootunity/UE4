// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

/************************************************************************
* Desc 	: 装备与身体部位的对应表，每个装备对应最多3个小部件，装备枚举为：BODY_PART
* Param	: 
* Return	: 
* Author	: WangPeng
* Time		: [3/1/2019 wp]
************************************************************************/

typedef struct tagItemPartData
{
    int32               EquipEnum;	        // 装备枚举
    TArray<FString>     BodyPartNameList;   // 身体部位名，最多3个
}FItemPartData;

class TABLEMODULE_API FItemPartTable : public STabBaseTable < FItemPartTable, FItemPartData>
{
public:
	FItemPartTable();
	virtual ~FItemPartTable();

public:
    virtual bool ReadTable(int32 nRow, int32& nCol) override;
};
