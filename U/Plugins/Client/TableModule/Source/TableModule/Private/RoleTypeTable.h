// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TableDataDefine.h"
#include "BaseTable.h"

/************************************************************************
* Desc 	:  角色、NPC类型表，处理模型、装备资源
* Param	: 
* Return	: 
* Author	: WangPeng
* Time		: [2/28/2019 wp]
************************************************************************/


typedef struct tagBodyPartFileData
{
	int32	RoleTypeID;
	FString BodyPartFileName;

public:
	tagBodyPartFileData() {}

	tagBodyPartFileData(int32 ID, FString name)
		:RoleTypeID(ID)
		,BodyPartFileName(name)
	{

	}
}FBodyPartFileData;


class TABLEMODULE_API RoleTypeTable : public STabBaseTable<RoleTypeTable, FRoleTypeData>
{
public:
	RoleTypeTable();
	virtual ~RoleTypeTable();

private:
    TMap<int32, FString>					m_mRoleType2BodyPart;
	TArray<FBodyPartFileData>				m_TBodyPartFileNameArray;

public:
    virtual bool                ReadTable(int32 nRow, int32& nCol) override;

public:
	TArray<FBodyPartFileData>				GetBodyPartFileNames()
	{
		return m_TBodyPartFileNameArray;
	}

private:
	bool									IsPartFileExisted(FString FileName);
};
