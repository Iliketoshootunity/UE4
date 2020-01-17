// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

typedef struct tagCameraCfgData
{
    tagCameraCfgData()
	{
		fArmLength = 600.f;
		fSocketOffsetZ = 300.f;
		fProbeSize = 24.f;
		bPawnControlRotation = false;
		bCameraLag = false;
		bCameraRotationLag = false;
		fCameraLagSpeed = 5.f;
		fCameraRotationLagSpeed = 5.f;
		bFollowPawnControlRotation = false;
		fFollowCameraRotationY = -20.f;

	}

	float fArmLength;
	float fSocketOffsetZ;
	float fProbeSize;
	bool bPawnControlRotation;
	bool bCameraLag;
	bool bCameraRotationLag;
	float fCameraLagSpeed;
	float fCameraRotationLagSpeed;
	bool bFollowPawnControlRotation;
	float fFollowCameraRotationY;

}FCameraCfgData;

/**
 * 
 */
class TABLEMODULE_API SCameraConfig : public SIniBaseTable<SCameraConfig, FCameraCfgData>
{
public:
    SCameraConfig();
	virtual ~SCameraConfig();

public:
    virtual bool				ReadTable(BaseUnitManager* pTableManager) override;

};
