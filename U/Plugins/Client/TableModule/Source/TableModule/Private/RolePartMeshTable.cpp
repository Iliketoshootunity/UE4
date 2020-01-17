// Fill out your copyright notice in the Description page of Project Settings.

#include "RolePartMeshTable.h"

FRolePartMeshTable::FRolePartMeshTable()
    : MTabBaseTable(this)
{
}

FRolePartMeshTable::~FRolePartMeshTable()
{
}

bool FRolePartMeshTable::ReadTable(int32 nRow, int32& nCol)
{
	FRolePartMeshData Data;

	nCol++; // Desc

    GetTableManager()->GetTabInteger(nRow, nCol++, 0, &Data.ID);

    GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, Data.SkeletalMesh);

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

    SetData(Data.ID, Data);

    return true;
}

//////////////////////////////////////////////////////////////////

