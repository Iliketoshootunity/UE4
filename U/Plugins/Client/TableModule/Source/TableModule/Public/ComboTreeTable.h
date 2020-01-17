#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "BaseTable.h"

/************************************************************************
* Desc 	: ComboTree±í
* Author	: XiaoHailin
* Time		: [8/10/2019 xhl]
************************************************************************/

typedef struct tagComboTreesTableInfo
{
    int32			ID;
	FString			Name;
	int32			MotionType;
	int32			FirstComboID;

}FComboTreesTableData;

class TABLEMODULE_API ComboTreeTable : public STabBaseTable<ComboTreeTable, FComboTreesTableData>
{
public:
	ComboTreeTable();
	virtual ~ComboTreeTable();

public:
    virtual bool				ReadTable(int32 nRow, int32& nCol) override;
};