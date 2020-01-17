// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTable.h"



SkillTable::SkillTable() :STabBaseTable(this)
{

}

SkillTable::~SkillTable()
{

}

bool SkillTable::ReadTable(int32 nRow, int32& nCol)
{
	FSkillTableData data;

	nCol++;

	if (!GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.SkillID))
		return false;
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.SkillName);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.TriggerType);
	GetTableManager()->GetTabString(nRow, nCol++, ANSI_NONE, data.FirstComboMontage);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.Kind);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.SkillNeedRace);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.SkillNeedWeapon);
	GetTableManager()->GetTabInteger(nRow, nCol++, INVALID_INDEX, data.FirstComboClipId);
	SetData(data.SkillID, data);

	return true;
}