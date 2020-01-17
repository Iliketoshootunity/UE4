// Fill out your copyright notice in the Description page of Project Settings.

#include "SLoadingScreenTable.h"
#include "SLoadingTable.h"

SLoadingScreenTable::SLoadingScreenTable()
	: STabBaseTable(this)
{
}

SLoadingScreenTable::~SLoadingScreenTable()
{
}

bool SLoadingScreenTable::ReadTable(int32 nRow, int32& nCol)
{
	nCol++;

	FLSTableData data;

	if (!GetTableManager()->GetTabInteger(nRow, nCol++, 0, data.ID))
		return false;

	FString FSLoadingID;
	//if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE_PARAM("0|0|0"), FSLoadingID)) // ID
	//	return false;

	int32 LoadingID1, LoadingID2, LoadingID3;

	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE_PARAM("0|0|0"), "%d|%d|%d", &LoadingID1, &LoadingID2, &LoadingID3);
	//UGameUtil::ue_sscanf(FSLoadingID, "%d|%d|%d", &LoadingID1, &LoadingID2, &LoadingID3);
	data.LoadingIDList.Add(LoadingID1);
	data.LoadingIDList.Add(LoadingID2);
	data.LoadingIDList.Add(LoadingID3);

	GetTableManager()->GetTabBool(nRow, nCol++, false, data.bForcePlay);

	SetData(data.ID, data);

	return true;

}

TArray<FLoadingAssetData> SLoadingScreenTable::GetLoadingAsset(int32 ID)
{
	TArray<FLoadingAssetData> _LoadingAssetList;
	if (!IsLoaded() || !SLoadingTable::Get()->IsLoaded())
		return _LoadingAssetList;

	const FLSTableData* pLSTableData = GetData(ID);
	if (pLSTableData == NULL)
		return _LoadingAssetList;

	for (auto LoadingAssetID : pLSTableData->LoadingIDList)
	{
		const FLoadingTableData* pLTableData = SLoadingTable::Get()->GetData(LoadingAssetID);
		if(!pLTableData) continue;

		FLoadingAssetData data;
		data.AssetName = pLTableData->AssetName;
		data.MiniDisplayTime = pLTableData->MiniDisplayTime;
		data.IsMovice = pLTableData->IsMovice;

		_LoadingAssetList.Add(data);
	}
	
	return _LoadingAssetList;
}

bool SLoadingScreenTable::IsForcePlay(int32 nID)
{
	TArray<FLoadingAssetData> _LoadingAssetList;
	if (!IsLoaded())
		return false;

	const FLSTableData* pLSTableData = GetData(nID);
	if (pLSTableData == NULL)
		return false;

	return pLSTableData->bForcePlay;
}
