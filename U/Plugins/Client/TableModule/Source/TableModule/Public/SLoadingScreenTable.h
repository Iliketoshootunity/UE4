// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"


/************************************************************************
* Desc 	: LoadingΩÁ√Ê≈‰÷√±Ì
* Author	: WangPeng
* Time		: [3/21/2019 wp]
************************************************************************/

typedef struct tagLoadingScreenTableData
{
	int32			ID;
	TArray<int32>	LoadingIDList;
	bool			bForcePlay;
}FLSTableData;

typedef struct tagLoadingAssetData
{
	FString			AssetName;
	int32			Mark;
	float			MiniDisplayTime;
	bool			IsMovice;
}FLoadingAssetData;

class TABLEMODULE_API SLoadingScreenTable : public STabBaseTable<SLoadingScreenTable, FLSTableData>
{
public:
	SLoadingScreenTable();
	virtual ~SLoadingScreenTable();

public:
	virtual bool				ReadTable(int32 nRow, int32& nCol) override;

	TArray<FLoadingAssetData>	GetLoadingAsset(int32 ID);
	bool						IsForcePlay(int32 nID);
};
