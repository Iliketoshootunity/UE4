// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemPartTable.h"

FItemPartTable::FItemPartTable()
    : STabBaseTable(this)
{
}

FItemPartTable::~FItemPartTable()
{
}

bool FItemPartTable::ReadTable(int32 nRow, int32& nCol)
{
	FItemPartData data;
    nCol++; // 排除第1列 说明

    if (!GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, &data.EquipEnum))
        return false;

    FString BodyPart1, BodyPart2, BodyPart3;
    GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, BodyPart1);	// BodyPart1
    data.BodyPartNameList.Add(BodyPart1);

    GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, BodyPart2); // BodyPart2
    data.BodyPartNameList.Add(BodyPart2);

    GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, BodyPart3); // BodyPart3
    data.BodyPartNameList.Add(BodyPart3);


    SetData(data.EquipEnum, data);

    return true;
}
