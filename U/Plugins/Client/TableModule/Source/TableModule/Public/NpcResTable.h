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
    * Desc 	: ��ȡRoleTypeTable�������
    * Param	: RoleTypeID : ����������(ÿ��character����һ��RoleTypeID)
    * Return	: �ɹ� �� const RoleTypeData* �� ʧ�� NULL
    * Author	: WangPeng
    * Time		: [3/5/2019 wp]
    ************************************************************************/
    const FRoleTypeData*                 GetRoleTypeData(int32 RoleTypeID);

	/************************************************************************
	* Desc 	: ��ȡ���岿������(BODY_PART_HAIR - BODY_PART_LEG)
	* Param	: inRoeTypeID : RoleType��ID��eBodyPart : BODY_PARTö��ֵ��nPartID : ��������Ĳ���ID
	* Return	: ��Ӧ������
	* Author	: WangPeng
	* Time		: [4/23/2019 wp]
	************************************************************************/
	const FRolePartMeshData*			 GetBodyPartData(int32 nRoleTypeID, int32 eBodyPart, int32 nPartID);

	/************************************************************************
	* Desc 	: ��ȡ���ⲿ��������(BODY_PART_LHAND_WEAPON BODY_PART_RHAND_WEAPON BODY_PART_HORSE)
	* Param	: inRoeTypeID : RoleType��ID��eBodyPart : BODY_PARTö��ֵ��nPartID : ��������Ĳ���ID
	* Return	: ��Ӧ������
	* Author	: WangPeng
	* Time		: [4/23/2019 wp]
	************************************************************************/
	const FExtraRolePartMeshData*		 GetExtraPartData(int32 inRoeTypeID, int32 eBodyPart, int32 nPartID);
};
