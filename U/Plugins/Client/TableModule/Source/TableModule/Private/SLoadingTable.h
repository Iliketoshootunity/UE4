// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

/************************************************************************
* Desc 	: Loading资源表类
* Author	: WangPeng
* Time		: [3/21/2019 wp]
************************************************************************/
typedef struct tagLoadingTableData
{
	int32			ID;
	FString			AssetName;
	float			MiniDisplayTime;		//最小显示时间
	float			DelayHideTime;			//延迟消失时间
	bool			IsMovice;
}FLoadingTableData;

class SLoadingTable : public STabBaseTable<SLoadingTable, FLoadingTableData>
{
public:
	SLoadingTable();
	virtual ~SLoadingTable();

public:
	virtual bool				ReadTable(int32 nRow, int32& nCol) override;
};
