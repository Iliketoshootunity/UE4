// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseConfig.h"

SBaseConfig::SBaseConfig()
	: SIniBaseTable(this, TEXT("Configs/BaseConfig.ini"))
{
}

SBaseConfig::~SBaseConfig()
{
}

bool SBaseConfig::ReadTable(BaseUnitManager* pTableManager)
{
	if (!pTableManager) return false;

	FBaseCfgData data;

	pTableManager->GetIniString(TEXT("ConfigFiles"), TEXT("GameConfigFile"), ANSI_NONE, data.GameConfigFile);
	pTableManager->GetIniString(TEXT("ConfigFiles"), TEXT("CameraConfigFile"), ANSI_NONE, data.CameraConfigFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("TextFile"), ANSI_NONE, data.TextFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("NpcResPath"), ANSI_NONE, data.NpcResPath);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("EquipPartFile"), ANSI_NONE, data.EquipPartFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("RoleTypeFile"), ANSI_NONE, data.RoleTypeFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("BodyPartFile"), ANSI_NONE, data.BodyPartFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("NpcsFile"), ANSI_NONE, data.NpcsFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("MapsFile"), ANSI_NONE, data.MapsFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("LoadingFile"), ANSI_NONE, data.LoadingFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("LoadingScreenFile"), ANSI_NONE, data.LoadingScreenFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("SkillFile"), ANSI_NONE, data.SkillFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("ComboTreeFile"), ANSI_NONE, data.ComboTreeFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("ComboClipFile"), ANSI_NONE, data.ComboClipFile);
	pTableManager->GetIniString(TEXT("SettingFiles"), TEXT("HitPerformaceFile"), ANSI_NONE, data.HitPerformaceFile);

	pTableManager->GetIniString(TEXT("UIAsserts"), TEXT("UI_LoadingScreen"), ANSI_NONE, data.UI_LoadingAssert);
	pTableManager->GetIniString(TEXT("UIAsserts"), TEXT("UI_Login"), ANSI_NONE, data.UI_LoginAssert);
	pTableManager->GetIniString(TEXT("UIAsserts"), TEXT("UI_Launcher"), ANSI_NONE, data.UI_LauncherAssert);
	pTableManager->GetIniString(TEXT("UIAsserts"), TEXT("UI_CSRole"), ANSI_NONE, data.UI_CSRoleAsset);

	FServerStatusData ServerStatusData;
	pTableManager->GetIniString(TEXT("ServerStatus"), TEXT("Good"), ANSI_NONE, ServerStatusData.ServerStatusAssetFile);
	pTableManager->GetIniString(TEXT("ServerStatus"), TEXT("GoodName"), ANSI_NONE, ServerStatusData.ServerStatusName);
	data.ServerStatusAsset.Add(1, ServerStatusData);

	pTableManager->GetIniString(TEXT("ServerStatus"), TEXT("Busy"), ANSI_NONE, ServerStatusData.ServerStatusAssetFile);
	pTableManager->GetIniString(TEXT("ServerStatus"), TEXT("BusyName"), ANSI_NONE, ServerStatusData.ServerStatusName);
	data.ServerStatusAsset.Add(2, ServerStatusData);

	pTableManager->GetIniString(TEXT("ServerStatus"), TEXT("Maintain"), ANSI_NONE, ServerStatusData.ServerStatusAssetFile);
	pTableManager->GetIniString(TEXT("ServerStatus"), TEXT("MaintainName"), ANSI_NONE, ServerStatusData.ServerStatusName);
	data.ServerStatusAsset.Add(3, ServerStatusData);

	SetData(data);

	return true;
}

const FServerStatusData* SBaseConfig::GetServerStatusData(int32 StatusIdx)
{
	const FBaseCfgData* pData = GetData();
	if (!pData) return NULL;

	return pData->ServerStatusAsset.Find(StatusIdx);
}
