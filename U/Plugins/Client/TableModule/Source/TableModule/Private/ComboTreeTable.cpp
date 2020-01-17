// Fill out your copyright notice in the Description page of Project Settings.

#include "ComboTreeTable.h"

ComboTreeTable::ComboTreeTable() : STabBaseTable(this)
{

}

ComboTreeTable::~ComboTreeTable()
{

}

bool ComboTreeTable::ReadTable(int32 nRow, int32& nCol)
{

	FComboTreesTableData data;

	nCol++;

	if (!GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.ID))
		return false;
	if (!GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.Name)) // ID
		return false;
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.MotionType);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.FirstComboID);
	SetData(data.ID, data);

	return true;
}
