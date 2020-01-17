// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GameConfig.h"

SGameConfig::SGameConfig()
    : SIniBaseTable(this)
{
}

SGameConfig::~SGameConfig()
{
}

bool SGameConfig::ReadTable(BaseUnitManager* pTableManager)
{
    if (!pTableManager) return false;

    FGameCfgData data;
    pTableManager->GetIniBool(TEXT("Server"), TEXT("Tcp"), false, data.bIsUseTcp);
    pTableManager->GetIniBool(TEXT("Server"), TEXT("Udp"), false, data.bIsUseUdp);
    pTableManager->GetIniString(TEXT("TCP"), TEXT("IP"), TEXT("0.0.0.0"), data.szTcpIP);
    pTableManager->GetIniInt(TEXT("TCP"), TEXT("Port"), 0, data.nTcpPort);
    pTableManager->GetIniString(TEXT("UDP"), TEXT("IP"), TEXT("0.0.0.0"), data.szUdpIP);
    pTableManager->GetIniInt(TEXT("UDP"), TEXT("Port"), 0, data.nUdpPort);
    pTableManager->GetIniBool(TEXT("Language"), TEXT("Chinese"), false, data.bChinese);
    pTableManager->GetIniBool(TEXT("Language"), TEXT("English"), false, data.bEnglish);

    SetData(data);

	return true;
}

