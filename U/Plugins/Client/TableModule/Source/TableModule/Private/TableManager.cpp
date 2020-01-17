// Fill out your copyright notice in the Description page of Project Settings.

#include "TableManager.h"
#include "BaseConfig.h"
#include "GameConfig.h"
#include "CameraConfig.h"
#include "BodyPartTable.h"
#include "RoleTypeTable.h"
#include "NpcResTable.h"
#include "NpcsTable.h"
#include "SLoadingTable.h"
#include "SLoadingScreenTable.h"
#include "ServerListConfig.h"
#include "MapsTable.h"
#include "ComboTreeTable.h"
#include "ComboClipTable.h"
#include "SkillTable.h"
#include "HitPerformace.h"

TableManager::TableManager()
{
	m_bLoaded = false;
}

TableManager::~TableManager()
{
	m_bLoaded = false;
}

void TableManager::Load()
{
	if (m_bLoaded) return;

	m_bLoaded = false;

	//顺序不能改变
	SBaseConfig::Get()->LoadTable();
	if (!SBaseConfig::Get()->IsLoaded())
		return;

	SGameConfig::Get()->LoadTable(SBaseConfig::Get()->GetData()->GameConfigFile);
	SCameraConfig::Get()->LoadTable(SBaseConfig::Get()->GetData()->CameraConfigFile);
	NpcResTable::Get()->LoadTable();
	NpcsTable::Get()->LoadTable(SBaseConfig::Get()->GetData()->NpcsFile);
	MapsTable::Get()->LoadTable(SBaseConfig::Get()->GetData()->MapsFile);
	SkillTable::Get()->LoadTable(SBaseConfig::Get()->GetData()->SkillFile);
	ComboClipTable::Get()->LoadTable(SBaseConfig::Get()->GetData()->ComboClipFile);
	HitPerformace::Get()->LoadTable(SBaseConfig::Get()->GetData()->HitPerformaceFile);

	SLoadingTable::Get()->LoadTable(SBaseConfig::Get()->GetData()->LoadingFile);
	SLoadingScreenTable::Get()->LoadTable(SBaseConfig::Get()->GetData()->LoadingScreenFile);

	ServerListConfig::Get()->LoadTable(TEXT("Configs/Update/ServerList.ini"));

	m_bLoaded = true;
}

void TableManager::UnLoad()
{
	m_bLoaded = false;
}
