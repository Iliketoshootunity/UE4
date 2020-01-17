// Fill out your copyright notice in the Description page of Project Settings.

#include "SubMapTable.h"
#include "BasicFunction.h"

SubMapTable::SubMapTable()
    : MTabBaseTable(this)
{
}

SubMapTable::~SubMapTable()
{
}

bool SubMapTable::ReadTable(int32 nRow, int32& nCol)
{
    FSubMapTableData data;

    FString Buffer;

    nCol++;

    if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.MapShowName)) // ID
        return false;

    if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.MapName)) // ID
        return false;

    if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE_PARAM("0|0|0|0"), Buffer))		// ������ StringID 
        return false;

    FBasicFunction::ue_sscanf(Buffer, "%d|%d|%d|%d", &data.MapRect.left, &data.MapRect.top, &data.MapRect.right, &data.MapRect.bottom);

    GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE_PARAM("0|0"), Buffer);		// ������ StringID 

	FBasicFunction::ue_sscanf(Buffer, "%d|%d", &data.RegionSize.width, &data.RegionSize.height);

    if (!GetTableManager()->GetTabFloat(nRow, nCol++, 0.0f, data.WorldtoMeters))
        return false;

	GetTableManager()->GetTabInteger(nRow, nCol++, 0, data.StreamMapX);

	GetTableManager()->GetTabInteger(nRow, nCol++, 0, data.StreamMapY);

    GetTableManager()->GetTabBool(nRow, nCol++, false, data.bDefaultInitialize);

    GetTableManager()->GetTabBool(nRow, nCol++, false, data.bDefaultShow);

    GetTableManager()->GetTabBool(nRow, nCol++, false, data.bHaveStreamMap);

    GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.SubMapPath);


    int32 ID = FBasicFunction::MakeU32(data.StreamMapX, data.StreamMapY);
    if (ID < 0)
    {
        return false;
    }  

    SetData(ID, data);

    return true;
}

void SubMapTable::GetDefaultInitMapRegionID(TArray<int32>& arr)
{
    FDataMap* pSubMapInfo = GetData();

    for (FDataMap::TIterator It(*pSubMapInfo); It; ++It)
    {
        FSubMapTableData& Info = It.Value();
        if (Info.bDefaultInitialize)
        {
            arr.Add(It.Key());
        }
    }
}

int32 SubMapTable::GetDefaultShowMapRegionID()
{
    FDataMap* pSubMapInfo = GetData();

    for (FDataMap::TIterator It(*pSubMapInfo); It; ++It)
    {
        FSubMapTableData& Info = It.Value();
        if (Info.bDefaultShow)
        {
            return It.Key();
        }
    }

    return -1;
}

void SubMapTable::GetDefaultMapRegionID(TArray<int32>& DefaultInitIDs, TArray<int32>& DefaultShowIDs)
{
    DefaultInitIDs.Empty();
    DefaultShowIDs.Empty();

    FDataMap* pSubMapInfo = GetData();

    for (FDataMap::TIterator It(*pSubMapInfo); It; ++It)
    {
        FSubMapTableData& Info = It.Value();
        if (Info.bDefaultInitialize)
        {
            DefaultInitIDs.Add(It.Key());
        }
        if (Info.bDefaultShow)
        {
            DefaultShowIDs.Add(It.Key());
        }
    }
}

bool SubMapTable::IsInLevel(int32 ID, FVector Location)
{
    const FSubMapTableData* Info = GetData(ID);
    if (!Info) return false;
    float _fPixelWidth = (Info->MapRect.right - Info->MapRect.left) * Info->WorldtoMeters;
    float _fPixelHeight = (Info->MapRect.bottom - Info->MapRect.top) * Info->WorldtoMeters;
    FVector2D _vMinPixelLoc = FVector2D(Info->StreamMapX * _fPixelWidth - _fPixelWidth, Info->StreamMapY * _fPixelHeight - _fPixelHeight);
    FVector2D _vMaxPixelLoc = FVector2D(Info->StreamMapX * _fPixelWidth, Info->StreamMapY * _fPixelHeight);

    if (Location.X >= _vMinPixelLoc.X && Location.X < _vMaxPixelLoc.X && Location.Y >= _vMinPixelLoc.Y && Location.Y < _vMaxPixelLoc.Y)
    {
        return true;
    }

    return false;
}

bool SubMapTable::IsInLevel(const FSubMapTableData* Info, const FVector Location)
{
    if (!Info) return false;
    float _fPixelWidth = (Info->MapRect.right - Info->MapRect.left) * Info->WorldtoMeters;
    float _fPixelHeight = (Info->MapRect.bottom - Info->MapRect.top) * Info->WorldtoMeters;
    FVector2D _vMinPixelLoc = FVector2D(Info->StreamMapX * _fPixelWidth - _fPixelWidth, Info->StreamMapY * _fPixelHeight - _fPixelHeight);
    FVector2D _vMaxPixelLoc = FVector2D(Info->StreamMapX * _fPixelWidth, Info->StreamMapY * _fPixelHeight);

    if (Location.X >= _vMinPixelLoc.X && Location.X < _vMaxPixelLoc.X && Location.Y >= _vMinPixelLoc.Y && Location.Y < _vMaxPixelLoc.Y)
    {
        return true;
    }

    return false;
}
