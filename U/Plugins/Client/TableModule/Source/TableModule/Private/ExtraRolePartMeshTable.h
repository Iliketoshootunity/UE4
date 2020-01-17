// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefine.h"
#include "BaseTable.h"
#include "TableDataDefine.h"


/************************************************************************
* Desc 	: Role 额外的挂在模型，此部件不需要合并骨骼模型【Weapon、Ride等】
* Author	: WangPeng
* Time		: [4/17/2019 wp]
************************************************************************/



class TABLEMODULE_API FExtraRolePartMeshTable : public MTabBaseTable<FExtraRolePartMeshTable, FExtraRolePartMeshData>
{
public:
	FExtraRolePartMeshTable();
	virtual ~FExtraRolePartMeshTable();

public:
	virtual bool ReadTable(int32 nRow, int32& nCol) override;
};
