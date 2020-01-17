// Fill out your copyright notice in the Description page of Project Settings.

#include "NpcsTable.h"

NpcsTable::NpcsTable()
    : STabBaseTable(this)
{
}

NpcsTable::~NpcsTable()
{
}

bool NpcsTable::ReadTable(int32 nRow, int32& nCol)
{
	FNpcsTableData data;

    nCol++;

    if (!GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.ID))
        return false;

    GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.TitleID);

    GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.NameID);

    if (!GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.Kind))
        return false;

    if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.HeadImage)) // ID
        return false;

    if (!GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.RoleTypeID))
        return false;

    GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.HairType);
    GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.FaceType);
    GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.ShoulderType);
    GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.BodyType);
    GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.BeltType);
    GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.LegType);
    GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.LWeaponType);
    GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.RWeaponType);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.HorseType);

    SetData(data.ID, data);

    return true;
}
