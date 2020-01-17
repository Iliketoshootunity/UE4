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

	bool		bIsUseTcp;			// �Ƿ�ʹ��TCP SOCKET, Ĭ��true
	bool		bIsUseUdp;			// �Ƿ�ʹ��UDP SOCKET��Ĭ��false
	FString		szTcpIP;			// TCP IP��Ĭ��127.0.0.1
	int32		nTcpPort;			// TCP �˿ڣ�Ĭ��0
	FString		szUdpIP;			// UDP IP��Ĭ��127.0.0.1
	int32		nUdpPort;			// UDP �˿ڣ�Ĭ��0

	bool		bChinese;			// ����
	bool		bEnglish;			// Ӣ��

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
