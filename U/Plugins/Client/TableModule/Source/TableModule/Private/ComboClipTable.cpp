// Fill out your copyright notice in the Description page of Project Settings.

#include "ComboClipTable.h"



ComboClipTable::ComboClipTable() :STabBaseTable(this)
{

}

ComboClipTable::~ComboClipTable()
{

}

bool ComboClipTable::ReadTable(int32 nRow, int32& nCol)
{
	FComboClipTableData data;
	nCol++;

	if (!GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.ID))
		return false;
	if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.Path))
		return false;
	GetTableManager()->GetTabFloat(nRow, nCol++, INVALID_INDEX, data.ChantLength);
	GetTableManager()->GetTabFloat(nRow, nCol++, INVALID_INDEX, data.ChantTimeNum);
	GetTableManager()->GetTabFloat(nRow, nCol++, INVALID_INDEX, data.ChargeLength);
	GetTableManager()->GetTabInteger(nRow, nCol++, 0, data.bMatchChargeRelease);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.HitPerformaceId);
	SetData(data.ID, data);

	return true;
}
