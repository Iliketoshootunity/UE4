// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

/************************************************************************
* Desc 	: 装备部件类
* Param	: 
* Return	: 
* Author	: WangPeng
* Time		: [2/28/2019 wp]
************************************************************************/

typedef struct tagBodyPartData
{
    FString PartName;	// 部件名
    FString BoneName;   // 骨骼名
	bool	IsExtra;	// 是否是额外部件，额外部件不需要合并骨骼模型
	int32   BodyPartIndex; //BODY_PART 枚举值
}BodyPartData;

class TABLEMODULE_API BodyPartTable : public STabBaseTable<BodyPartTable, BodyPartData>
{
public:
	BodyPartTable();
	virtual ~BodyPartTable();

public:
    virtual bool ReadTable(int32 nRow, int32& nCol) override;

	/************************************************************************
	* Desc 	: 获取部件资源数据
	* Param	: BodyPartName: 部件名字
	* Return	: 
	* Author	: WangPeng
	* Time		: [4/22/2019 wp]
	************************************************************************/
	const BodyPartData*			GetTableData(FString BodyPartName);

	/************************************************************************
	* Desc 	: 获取部件资源数据
	* Param	: BodyPartIndex ： BODY_PART 枚举值
	* Return	: 
	* Author	: WangPeng
	* Time		: [4/22/2019 wp]
	************************************************************************/
	const BodyPartData*			GetTableData(int32 BodyPartIndex);
private:
	TMap<FString, int32>		m_TName2IndexMap;
};
