// Fill out your copyright notice in the Description page of Project Settings.

#include "MapsTable.h"
#include "BasicFunction.h"

MapsTable::MapsTable()
    : STabBaseTable(this)
{
}

MapsTable::~MapsTable()
{
}

bool MapsTable::ReadTable(int32 nRow, int32& nCol)
{
    FMapsTableData data;

    nCol++;

    if (!GetTableManager()->GetTabInteger(nRow, nCol++, 0, data.ID))
        return false;

    if (!GetTableManager()->GetTabInteger(nRow, nCol++, 0, data.ShowMapNameID))
        return false;

    if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.MapName)) // ID
        return false;

    FString szBuffer;
    if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE_PARAM("0|0|0|0"), szBuffer)) // ID
        return false;

	FBasicFunction::ue_sscanf(szBuffer, "%d|%d|%d|%d", &data.MapRect.left, &data.MapRect.top, &data.MapRect.right, &data.MapRect.bottom);

    if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE_PARAM("0|0"), szBuffer)) // ID
        return false;

	FBasicFunction::ue_sscanf(szBuffer, "%d|%d", &data.RegionSize.width, &data.RegionSize.height);

    if (!GetTableManager()->GetTabInteger(nRow, nCol++, 0, data.WorldtoMeters))
        return false;

    GetTableManager()->GetTabBool(nRow, nCol++, false, &data.HaveStreamMap);

	GetTableManager()->GetTabBool(nRow, nCol++, false, &data.IsNineGridLevel);

    GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.SubMapPath); // ID
    if (data.HaveStreamMap && !data.SubMapPath.IsEmpty())
    {
        data.pSubMapTable = new SubMapTable();
        data.pSubMapTable->LoadTable(data.SubMapPath);
    }

	if (!GetTableManager()->GetTabInteger(nRow, nCol++, INDEX_NONE, data.LoadingScreenID))
		return false;

    SetData(data.ID, data);

    return true;
}

bool MapsTable::IsHaveSubMap(int32 MapID)
{
    const FMapsTableData* pMapInfo = GetData(MapID);
    if (pMapInfo == NULL) return false;

    return pMapInfo->HaveStreamMap;
}

bool MapsTable::IsNineGridMap(int32 MapID)
{
	const FMapsTableData* pMapInfo = GetData(MapID);
	if (pMapInfo == NULL) return false;

	return pMapInfo->IsNineGridLevel;
}

const SubMapTable* MapsTable::GetSubMapTable(int32 MapID)
{
    const FMapsTableData* pMapInfo = GetData(MapID);
    if (pMapInfo == NULL) return NULL;
    if (!pMapInfo->HaveStreamMap) return NULL;
    return pMapInfo->pSubMapTable;
}

const FSubMapTableData* MapsTable::GetSubMapData(int32 MapID, int32 SubMapIndex)
{
    const FMapsTableData* pMapInfo = GetData(MapID);
    if (pMapInfo == NULL) return NULL;
    if (!pMapInfo->HaveStreamMap) return NULL;
    if (pMapInfo->pSubMapTable == NULL) return NULL;

    return pMapInfo->pSubMapTable->GetData(SubMapIndex);
}

FString MapsTable::GetSubMapName(int32 MapID, int32 SubMapIndex)
{
    const FMapsTableData* pMapInfo = GetData(MapID);
    if (pMapInfo == NULL || !pMapInfo->HaveStreamMap) return ANSI_NONE;
    if (pMapInfo->pSubMapTable == NULL) return ANSI_NONE;
    const FSubMapTableData* pSubmapInfp = pMapInfo->pSubMapTable->GetData(SubMapIndex);
    return pSubmapInfp == NULL ? ANSI_NONE : pSubmapInfp->MapName;

}

uint64 MapsTable::CalCurrentSubLevelID(int32 MapID, FVector Location)
{
    SubMapTable* pSubMapTable = (SubMapTable*)GetSubMapTable(MapID);
    if (pSubMapTable == NULL) return -1;

    SubMapTable::FDataMap* pSubMapLst = pSubMapTable->GetData();
    for (SubMapTable::FDataMap::TIterator It(*pSubMapLst); It; ++It)
    {
        FSubMapTableData* Info = &It.Value();

        if (pSubMapTable->IsInLevel(Info, Location))

        {
            return It.Key();
        }
    }

    return -1;
}
