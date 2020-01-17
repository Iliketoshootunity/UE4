// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtraRolePartMeshTable.h"
#include "BasicFunction.h"


FExtraRolePartMeshTable::FExtraRolePartMeshTable()
	: MTabBaseTable(this)
{

}

FExtraRolePartMeshTable::~FExtraRolePartMeshTable()
{

}

bool FExtraRolePartMeshTable::ReadTable(int32 nRow, int32& nCol)
{
	FExtraRolePartMeshData Data;

	nCol++; // Desc

	GetTableManager()->GetTabInteger(nRow, nCol++, 0, &Data.ID);

	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.SkeletalMesh);

	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.BoneName);

	FMaterialData   MaterialData;
	if (GetTableManager()->GetTabMaterial(nRow, nCol++, ANSI_NONE_PARAM("|"), ANSI_NONE_PARAM("-1| "), MaterialData)) // Material
	{
		Data.MeshMaterial.Add(MaterialData);
	}

	if (GetTableManager()->GetTabMaterial(nRow, nCol++, ANSI_NONE_PARAM("|"), ANSI_NONE_PARAM("-1| "), MaterialData)) // Material
	{
		Data.MeshMaterial.Add(MaterialData);
	}

	if (GetTableManager()->GetTabMaterial(nRow, nCol++, ANSI_NONE_PARAM("|"), ANSI_NONE_PARAM("-1| "), MaterialData)) // Material
	{
		Data.MeshMaterial.Add(MaterialData);
	}

	FString Location, Rotator;
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE_PARAM("0.f|0.f|0.f"), Location);
	FBasicFunction::ue_sscanf(Location, "%f|%f|%f", &Data.OffsetLocation.X, &Data.OffsetLocation.Y, &Data.OffsetLocation.Z);

	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE_PARAM("0.f|0.f|0.f"), Rotator);
	FBasicFunction::ue_sscanf(Rotator, "%f|%f|%f", &Data.Rotator.Pitch, &Data.Rotator.Yaw, &Data.Rotator.Roll);

	GetTableManager()->GetTabFloat(nRow, nCol++, 1.0f, &Data.Scale);

	SetData(Data.ID, Data);

	return true;
}
