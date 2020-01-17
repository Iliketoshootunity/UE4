// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

/************************************************************************
* Desc 	: Npcs±í£¬°üº¬Npc¡¢Monster¡¢Pet
* Author	: WangPeng
* Time		: [3/6/2019 wp]
************************************************************************/

typedef struct tagNpcsTableInfo
{
    int32			ID;
    int32			TitleID;
    int32			NameID;
    int32		    Kind;
    FString		    HeadImage;
    int32           RoleTypeID;

	int32			HairType;
    int32   	    FaceType;
	int32           ShoulderType;
	int32			BodyType;
	int32			BeltType;
	int32			LegType;
	int32           LWeaponType;
	int32           RWeaponType;
    int32           HorseType;
}FNpcsTableData;

class TABLEMODULE_API NpcsTable : public STabBaseTable<NpcsTable, FNpcsTableData>
{
public:
	NpcsTable();
	virtual ~NpcsTable();

public:
    virtual bool				ReadTable(int32 nRow, int32& nCol) override;
};
