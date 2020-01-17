// Fill out your copyright notice in the Description page of Project Settings.

#include "BodyPartTable.h"

BodyPartTable::BodyPartTable()
    : STabBaseTable(this)
{
}

BodyPartTable::~BodyPartTable()
{
}

bool BodyPartTable::ReadTable(int32 nRow, int32& nCol)
{
    BodyPartData data;
    nCol++; // 排除第1列 说明

    if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.PartName))	// ID
        return false;

    if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.BoneName)) // PartName
        return false;

	GetTableManager()->GetTabBool(nRow, nCol++, false, &data.IsExtra);

	if (!GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, &data.BodyPartIndex)) // BoduPartIndex
		return false;

	m_TName2IndexMap.Add(data.PartName, data.BodyPartIndex);

    SetData(data.BodyPartIndex, data);

    return true;
}

const BodyPartData* BodyPartTable::GetTableData(FString BodyPartName)
{
	if (!m_TName2IndexMap.Contains(BodyPartName))
		return nullptr;

	int32 Index = m_TName2IndexMap.FindRef(BodyPartName);
	if (Index == INVALID_INDEX)
		return nullptr;

	return GetData(Index);
}

const BodyPartData* BodyPartTable::GetTableData(int32 BodyPartIndex)
{
	return GetData(BodyPartIndex);
}
