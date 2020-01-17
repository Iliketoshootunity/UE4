// Fill out your copyright notice in the Description page of Project Settings.

#include "RoleTypeTable.h"
#include "BodyPartTable.h"

RoleTypeTable::RoleTypeTable()
	: STabBaseTable(this)
{
	m_mRoleType2BodyPart.Empty();
}

RoleTypeTable::~RoleTypeTable()
{
	m_mRoleType2BodyPart.Empty();
}

bool RoleTypeTable::ReadTable(int32 nRow, int32& nCol)
{
	FRoleTypeData data;

	nCol++; // ÅÅ³ýµÚ1ÁÐ Ãû³Æ

	if (!GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, &data.ID))	        // ID
		return false;

	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.FileName);	    // FilePath

	if (!data.FileName.IsEmpty())
	{
		m_TBodyPartFileNameArray.Add(FBodyPartFileData(data.ID, data.FileName));
	}

	if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.BlueprintName))		// BlueprintName
		return false;

	if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.SkeletonName))		    // MeshName
		return false;

	if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.AnimInstanceName))		// AnimInstanceName
		return false;

	if (!GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.AnimationMode))		// AnimationMode
		return false;

	if (!GetTableManager()->GetTabFloat(nRow, nCol++, 1.0f, &data.Scale))	// Scale
		return false;

	FMaterialData   MaterialData;
	if (GetTableManager()->GetTabMaterial(nRow, nCol++, ANSI_NONE_PARAM("|"), ANSI_NONE_PARAM("-1| "), MaterialData)) // Material
	{
		data.Material.Add(MaterialData);
	}

	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.NormalHurtMontage);

	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.DiaupHurtMontage);

	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.BigHurtMontage);

	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.DeadMontage);

	SetData(data.ID, data);

	return true;
}

bool RoleTypeTable::IsPartFileExisted(FString FileName)
{
	for (auto PartFileIt : m_TBodyPartFileNameArray)
	{
		if (PartFileIt.BodyPartFileName == FileName)
			return true;
	}

	return false;
}
