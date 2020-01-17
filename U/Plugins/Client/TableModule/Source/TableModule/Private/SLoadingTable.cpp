// Fill out your copyright notice in the Description page of Project Settings.

#include "SLoadingTable.h"

SLoadingTable::SLoadingTable()
	: STabBaseTable(this)
{
}

SLoadingTable::~SLoadingTable()
{
}

bool SLoadingTable::ReadTable(int32 nRow, int32& nCol)
{
	nCol++;

	FLoadingTableData data;

	if (!GetTableManager()->GetTabInteger(nRow, nCol++, 0, data.ID))
		return false;

	if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.AssetName)) // ID
		return false;

	GetTableManager()->GetTabFloat(nRow, nCol++, 0.0f, data.MiniDisplayTime);

	GetTableManager()->GetTabFloat(nRow, nCol++, 0.0f, data.DelayHideTime);

	GetTableManager()->GetTabBool(nRow, nCol++, false, data.IsMovice);

	SetData(data.ID, data);

	return true;
}
