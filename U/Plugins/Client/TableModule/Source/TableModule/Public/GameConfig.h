// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

typedef struct tagGameCfgData
{
    tagGameCfgData()
	{
		szTcpIP.Empty();
		nTcpPort = 0;
		szUdpIP.Empty();
		nUdpPort = 0;
		bIsUseTcp = true;
		bIsUseUdp = false;
		bChinese = true;
        bEnglish = false;
	}

	bool		bIsUseTcp;			// 是否使用TCP SOCKET, 默认true
	bool		bIsUseUdp;			// 是否使用UDP SOCKET，默认false
	FString		szTcpIP;			// TCP IP，默认127.0.0.1
	int32		nTcpPort;			// TCP 端口，默认0
	FString		szUdpIP;			// UDP IP，默认127.0.0.1
	int32		nUdpPort;			// UDP 端口，默认0

	bool		bChinese;			// 中文
	bool		bEnglish;			// 英文

} FGameCfgData;


/**
 * 
 */

class TABLEMODULE_API SGameConfig : public SIniBaseTable<SGameConfig, FGameCfgData>
{
public:
    SGameConfig();
	virtual ~SGameConfig();

public:
    virtual bool				ReadTable(BaseUnitManager* pTableManager) override;
};
