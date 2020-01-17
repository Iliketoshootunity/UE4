// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGameDefine.h"
#include "SDBCommon.h"

#pragma pack(push, 1)

// Player Save Data
typedef struct tagDBBaseColSet
{
	char szAccountName[MAX_ACCOUNT];
	char szRoleName[MAX_NAME_LEN];

}DBBaseColSet;

#pragma pack(pop)

typedef struct _DBProcHeader
{
	uint32 ulNetID;
	uint32 ulPlayerIdx;

	char szAccName[MAX_ACCOUNT];
	int ProcType;
	int SubProcType;
	int SubSubProcType;
}DBProcHeader;

