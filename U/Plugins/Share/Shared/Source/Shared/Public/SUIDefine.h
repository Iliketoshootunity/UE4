// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SUIDefine.generated.h"

/************************************************************************
* Desc 	: UI���ͣ�UI��ʶ��
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
		UI_LoadingScreen,	// loading����
		UI_Controller,		// ���ؽ���
		UI_Login,			// ��½����
		UI_Launcher,		// ��Դ���½���
		UI_CSRole,			// ������ɫ&ѡ���ɫ
		UI_All				// ȫ��
	};
}

UENUM(BlueprintType)
namespace ELoadingMark
{
	enum Type
	{
		Mark_None,
		Mark_Launcher,
		Mark_Login,			// ��½����
		Mark_ChangeMap,
		Mark_Num
	};
}

/************************************************************************
* Desc 	: ������״̬�����û��õ�
* Author	: WangPeng
* Time		: [3/28/2019 wp]
************************************************************************/
UENUM(BlueprintType)
namespace EServerStatus
{
	enum Type
	{
		Status_None,
		Status_Good,			// ����
		Status_Busy,			// ��æ
		Status_Maintain,		// ά��
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