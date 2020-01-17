// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"
#include "TableDataDefine.h"

/************************************************************************
* Desc 	: 部件配置表类，管理需要合并骨骼模型的部件配置表。
* Author	: WangPeng
* Time		: [2/28/2019 wp]
************************************************************************/



class TABLEMODULE_API FRolePartMeshTable : public MTabBaseTable<FRolePartMeshTable, FRolePartMeshData>
{
public:
	FRolePartMeshTable();
    virtual ~FRolePartMeshTable();

public:
    virtual bool ReadTable(int32 nRow, int32& nCol) override;
};



