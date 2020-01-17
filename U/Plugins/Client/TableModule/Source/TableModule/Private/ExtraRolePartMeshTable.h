// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefine.h"
#include "BaseTable.h"
#include "TableDataDefine.h"


/************************************************************************
* Desc 	: Role ����Ĺ���ģ�ͣ��˲�������Ҫ�ϲ�����ģ�͡�Weapon��Ride�ȡ�
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
