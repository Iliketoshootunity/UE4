#pragma once

#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "BaseTable.h"

/************************************************************************
* Desc 	: HitPerformace±í
* Author	: XiaoHailin
* Time		: [26/12/2019 xhl]
************************************************************************/

typedef struct tagHitPerformaceTableInfo
{
	int32			ID;
	int32			HitType;
	int32			AirHitType;
	int32			ControlType;
	int32			HitChannel;
	int32			bCanHitAir;
	int32			bCanHitGround;
	float			HardLevelCut;
	float			RootMontionScale;
	float			PushDistance;
	float			AttackerPauseDelayFrame;
	float			AttackerPauseDurationFrame;
	float			SuffererPauseDelayFrame;
	float			SuffererPauseDurationFrame;
	FString			HitWoodFX;
	FString			HitEarthFX;
	FString			HitMetalFX;
	FString			HitMeatFX;
	FString			HitStoneFX;
	FString			HitFXRotate;
	FString			BlockedFX;
	FString			BreakBlockedFX;
	FString			WeaknessHittedFX;

	FString			CameraShake;

}FHitPerformaceTableData;

class TABLEMODULE_API HitPerformace : public STabBaseTable<HitPerformace, FHitPerformaceTableData>
{
public:
	HitPerformace();
	virtual ~HitPerformace();
public:
	virtual bool				ReadTable(int32 nRow, int32& nCol) override;
};