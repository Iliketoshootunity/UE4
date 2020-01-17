// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

/**
 *
 */

typedef struct tagServerStatusData
{
	FString		ServerStatusAssetFile;
	FString		ServerStatusName;
}FServerStatusData;

typedef struct tagBaseCfgData
{
	tagBaseCfgData()
	{
		TextFile.Empty();
		GameConfigFile.Empty();
		CameraConfigFile.Empty();
	}

	//Configs
	FString     GameConfigFile;             // Configs/GameConfig.ini
	FString     CameraConfigFile;           // Configs/CameraConfig.ini

	//Configs/Settings
	FString		TextFile;			        // Configs/Settings/Text.txt
	FString     NpcResPath;                 // Configs/Settings/NpcRes/
	FString     RoleTypeFile;               // Configs/Settings/NpcRes/RoleType.txt
	FString     BodyPartFile;               // Configs/Settings/NpcRes/BodyPart.txt
	FString     EquipPartFile;              // Configs/Settings/NpcRes/EquipPartUnit.txt
	FString     NpcsFile;                   // Configs/Settings/Npcs.txt
	FString     MapsFile;                   // Configs/Settings/Maps/Maps.txt
	FString		LoadingFile;				// Configs/Settings/Loading.txt
	FString	    LoadingScreenFile;			// Configs/Settings/LoadingScreen.txt 
	FString		SkillFile;					// Configs/Settings/Combo/Skill.txt
	FString		ComboTreeFile;				// Configs/Settings/Combo/ComboTree.txt 
	FString		ComboClipFile;				// Configs/Settings/Combo/ComboClip.txt
	FString		HitPerformaceFile;			// Configs/Settings/Combo/HitPerformaceFile.txt

//UIAsserts 
	FString     UI_LoadingAssert;
	FString     UI_LoginAssert;
	FString		UI_LauncherAssert;
	FString		UI_CSRoleAsset;
	//serverstatus
	TMap<int32, FServerStatusData>	ServerStatusAsset;

} FBaseCfgData;

class TABLEMODULE_API SBaseConfig : public SIniBaseTable<SBaseConfig, FBaseCfgData>
{
public:
	SBaseConfig();
	virtual ~SBaseConfig();

public:
	virtual bool				ReadTable(BaseUnitManager* pTableManager) override;

	const FServerStatusData* GetServerStatusData(int32 StatusIdx);
};
