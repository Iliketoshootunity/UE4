// Fill out your copyright notice in the Description page of Project Settings.

#include "NpcResTable.h"
#include "BodyPartTable.h"
#include "BaseConfig.h"
#include "ItemPartTable.h"
#include "RolePartMeshTable.h"
#include "RolePartMeshManager.h"
#include "RoleTypeTable.h"

NpcResTable::NpcResTable()
{
}

NpcResTable::~NpcResTable()
{
}

bool NpcResTable::LoadTable()
{
    if (!SBaseConfig::Get()->IsLoaded())
        return false;

	FItemPartTable::Get()->LoadTable(SBaseConfig::Get()->GetData()->EquipPartFile);

    BodyPartTable::Get()->LoadTable(SBaseConfig::Get()->GetData()->BodyPartFile);

    RoleTypeTable::Get()->LoadTable(SBaseConfig::Get()->GetData()->RoleTypeFile);

    //需要RolePartTable、BodyPartTable两个关联表的数据，必须在这两个表后面加载
	FRolePartMeshManager::Get()->LoadTable();

    return true;
}


const FRoleTypeData* NpcResTable::GetRoleTypeData(int32 RoleTypeID)
{
	return RoleTypeTable::Get()->GetData(RoleTypeID);	
}

const FRolePartMeshData* NpcResTable::GetBodyPartData(int32 nRoleTypeID, int32 eBodyPart, int32 nPartID)
{
	return FRolePartMeshManager::Get()->GetBodypartData(nRoleTypeID, eBodyPart, nPartID);
}

const FExtraRolePartMeshData* NpcResTable::GetExtraPartData(int32 inRoeTypeID, int32 eBodyPart, int32 nPartID)
{
	return FRolePartMeshManager::Get()->GetExtraPartData(inRoeTypeID, eBodyPart, nPartID);
}
