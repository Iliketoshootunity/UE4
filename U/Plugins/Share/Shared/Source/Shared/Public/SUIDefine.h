// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SUIDefine.generated.h"

/************************************************************************
* Desc 	: UI类型，UI标识符
* Param	: 
* Return	: 
* Author	: WangPeng
* Time		: [2/25/2019 wp]
************************************************************************/
UENUM(BlueprintType)
namespace EUIType
{
	enum Type
	{
		UI_None,
		UI_LoadingScreen,	// loading界面
		UI_Controller,		// 主控界面
		UI_Login,			// 登陆界面
		UI_Launcher,		// 资源更新界面
		UI_CSRole,			// 创建角色&选择角色
		UI_All				// 全部
	};
}

UENUM(BlueprintType)
namespace ELoadingMark
{
	enum Type
	{
		Mark_None,
		Mark_Launcher,
		Mark_Login,			// 登陆界面
		Mark_ChangeMap,
		Mark_Num
	};
}

/************************************************************************
* Desc 	: 服务器状态，配置会用到
* Author	: WangPeng
* Time		: [3/28/2019 wp]
************************************************************************/
UENUM(BlueprintType)
namespace EServerStatus
{
	enum Type
	{
		Status_None,
		Status_Good,			// 良好
		Status_Busy,			// 繁忙
		Status_Maintain,		// 维护
		Status_Num
	};
}

USTRUCT(BlueprintType)
struct SHARED_API FTileItemData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Category = TileItem, EditAnywhere)
	FString ItemName;

	UPROPERTY(Category = TileItem, EditAnywhere)
	int32 ServerPort;

	UPROPERTY(Category = TileItem, EditAnywhere)
	FString ServerIP;
};