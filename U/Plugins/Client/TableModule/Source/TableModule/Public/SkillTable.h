#pragma once
#pragma once

#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "BaseTable.h"

/************************************************************************
* Desc 	: Skill±í
* Author	: XiaoHailin
* Time		: [14/10/2019 xhl]
************************************************************************/

typedef struct tagSkillTableInfo
{
	int32			Index;
	int32			SkillID;
	FString			SkillName;
	int32			TriggerType;
	FString			FirstComboMontage;
	int32			Kind;
	int32			SkillNeedRace;
	int32			SkillNeedWeapon;
	int32			FirstComboClipId;
}FSkillTableData;

class TABLEMODULE_API SkillTable : public STabBaseTable<SkillTable, FSkillTableData>
{
public:
	SkillTable();
	virtual ~SkillTable();
public:
	virtual bool				ReadTable(int32 nRow, int32& nCol) override;
};