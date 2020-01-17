#pragma once

#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "BaseTable.h"

/************************************************************************
* Desc 	: ComboClip±í
* Author	: XiaoHailin
* Time		: [8/10/2019 xhl]
************************************************************************/

typedef struct tagComboClipTableInfo
{
	int32			ID;
	FString			Path;
	float			ChantLength;
	float			ChantTimeNum;
	float			ChargeLength;
	int				bMatchChargeRelease;
	int32			HitPerformaceId;

}FComboClipTableData; 

class TABLEMODULE_API ComboClipTable : public STabBaseTable<ComboClipTable, FComboClipTableData>
{
public:
	ComboClipTable();
	virtual ~ComboClipTable();
public:
	virtual bool				ReadTable(int32 nRow, int32& nCol) override;
};
