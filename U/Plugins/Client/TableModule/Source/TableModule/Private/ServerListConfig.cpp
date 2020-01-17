// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerListConfig.h"
#include <ConfigCacheIni.h>

ServerListConfig::ServerListConfig()
	: SIniBaseTable(this)
{
}

ServerListConfig::~ServerListConfig()
{
}

bool ServerListConfig::ReadTable(BaseUnitManager* pTableManager)
{
	FData mapData;

	FConfigSection* pSection = pTableManager->GetIniSectionPrivate(TEXT("ServerList"), false, true);
	if (!pSection) return false;

	TArray<FString> ServerDatas;
	pSection->MultiFind(TEXT("Servers"), ServerDatas);
	if (ServerDatas.Num())
	{
		
		for (FString& ServerData : ServerDatas)
		{
			FServerListData Data;

			FParse::Value(*ServerData, TEXT("Idx="), Data.Idx);
			FParse::Value(*ServerData, TEXT("Count="), Data.Count);
			FParse::Value(*ServerData, TEXT("Asset="), Data.ItemAssetFile);
			FParse::Value(*ServerData, TEXT("AreaName="), Data.AreaName);

			TArray<FString> ServerInfos;
			ServerData.ParseIntoArray(ServerInfos, TEXT(";"));
			for (FString& ServerInfo : ServerInfos)
			{
				FServerData _Data;
				if (FParse::Value(*ServerInfo, TEXT("Name="), _Data.Name) &&
					FParse::Value(*ServerInfo, TEXT("Status="), _Data.Status) &&
					FParse::Value(*ServerInfo, TEXT("IP="), _Data.IP) &&
					FParse::Value(*ServerInfo, TEXT("Port="), _Data.Port))
				{
					Data.ArrServerData.Add(_Data);
				}
			}

			mapData.Add(Data.Idx, Data);
		}
	}

	SetData(mapData);

	return true;
}
