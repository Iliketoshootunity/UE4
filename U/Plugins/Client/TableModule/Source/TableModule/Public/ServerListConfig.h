// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

/**
 * 
 */
typedef struct tagServerData
{
	FString Name;
	int32   Status;
	FString IP;
	int32	Port;
}FServerData;

typedef struct tagServerListData
{
	int32 Idx;
	int32 Count;
	FString AreaName;
	FString ItemAssetFile;
	TArray<FServerData> ArrServerData;
}FServerListData;


class TABLEMODULE_API ServerListConfig : public SIniBaseTable<ServerListConfig, TMap<int32,FServerListData>>
{
public:
	ServerListConfig();
	virtual ~ServerListConfig();

public:
	virtual bool				ReadTable(BaseUnitManager* pTableManager) override;
};
