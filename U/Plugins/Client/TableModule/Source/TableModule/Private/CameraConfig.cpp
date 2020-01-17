// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CameraConfig.h"

SCameraConfig::SCameraConfig()
    : SIniBaseTable(this)
{
}

SCameraConfig::~SCameraConfig()
{
}

bool SCameraConfig::ReadTable(BaseUnitManager* pTableManager)
{
    if (!pTableManager) return false;

    FCameraCfgData data;
    pTableManager->GetIniFloat(TEXT("Camera"), TEXT("ArmLength"), 600.0f, data.fArmLength);
    pTableManager->GetIniFloat(TEXT("Camera"), TEXT("SocketOffsetZ"), 300.0f, data.fSocketOffsetZ);
    pTableManager->GetIniFloat(TEXT("Camera"), TEXT("ProbeSize"), 24.0f, data.fProbeSize);
    pTableManager->GetIniBool(TEXT("Camera"), TEXT("PawnControlRotation"), false, data.bPawnControlRotation);
    pTableManager->GetIniBool(TEXT("Camera"), TEXT("CameraLag"), false, data.bCameraLag);
    pTableManager->GetIniBool(TEXT("Camera"), TEXT("CameraRotationLag"), false, data.bCameraRotationLag);
    pTableManager->GetIniFloat(TEXT("Camera"), TEXT("CameraLagSpeed"), 5.0f, data.fCameraLagSpeed);
    pTableManager->GetIniFloat(TEXT("Camera"), TEXT("CameraRotationLagSpeed"), 5.0f, data.fCameraRotationLagSpeed);
    pTableManager->GetIniBool(TEXT("Camera"), TEXT("FollowPawnControlRotation"), false, data.bFollowPawnControlRotation);
    pTableManager->GetIniFloat(TEXT("Camera"), TEXT("FollowCameraRotationY"), 5.0f, data.fFollowCameraRotationY);

    SetData(data);

	return true;
}

