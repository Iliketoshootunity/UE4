// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSingleton.h"
#include "TableDataDefine.h"

/**
 * 
 */

class TABLEMODULE_API NpcResTable : public SSingleton<NpcResTable>
{
public:
    NpcResTable();
	virtual ~NpcResTable();

public:
    bool                                LoadTable();

    /************************************************************************
    * Desc 	: 获取RoleTypeTable表的数据
    * Param	: RoleTypeID : 表数据索引(每个character都有一个RoleTypeID)
    * Return	: 成功 ： const RoleTypeData* ， 失败 NULL
    * Author	: WangPeng
    * Time		: [3/5/2019 wp]
    ************************************************************************/
    const FRoleTypeData*                 GetRoleTypeData(int32 RoleTypeID);

	/************************************************************************
	* Desc 	: 获取身体部件数据(BODY_PART_HAIR - BODY_PART_LEG)
	* Param	: inRoeTypeID : RoleType表ID；eBodyPart : BODY_PART枚举值；nPartID : 部件表里的部件ID
	* Return	: 对应的数据
	* Author	: WangPeng
	* Time		: [4/23/2019 wp]
	************************************************************************/
	const FRolePartMeshData*			 GetBodyPartData(int32 nRoleTypeID, int32 eBodyPart, int32 nPartID);

	/************************************************************************
	* Desc 	: 获取额外部件的数据(BODY_PART_LHAND_WEAPON BODY_PART_RHAND_WEAPON BODY_PART_HORSE)
	* Param	: inRoeTypeID : RoleType表ID；eBodyPart : BODY_PART枚举值；nPartID : 部件表里的部件ID
	* Return	: 对应的数据
	* Author	: WangPeng
	* Time		: [4/23/2019 wp]
	************************************************************************/
	const FExtraRolePartMeshData*		 GetExtraPartData(int32 inRoeTypeID, int32 eBodyPart, int32 nPartID);
};
