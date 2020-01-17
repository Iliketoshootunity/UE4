// Fill out your copyright notice in the Description page of Project Settings.


#include "CSPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseSystem/Ext/MathfExtLibrary.h"
#include "BaseSystem/Ext/ActorInterpMovementComponent.h"



ACSPlayer::ACSPlayer()
{
}



void ACSPlayer::OnRotateToTagrget(float RotateTime /*= 0*/)
{
	float SourceYaw = CacheSourceYaw;
	float TargetYaw = 0;
	float Time = RotateTime;
	if (CacheTargetYaw != -9999)
	{
		TargetYaw = CacheTargetYaw;

	}
	else
	{
		//无攻击锁定下的
		if (CacheRockerX == 0 && CacheRockerY == 0)
		{
			return;
		}
		//计算目标旋转
		float ControllerYaw = CacheControllerYaw;
		float NewControllerYaw = 0;
		UKismetMathLibrary::FMod(ControllerYaw, 360, NewControllerYaw);
		float Angle = UMathfExtLibrary::VectorAngle2D(FVector2D(CacheRockerX, CacheRockerY), FVector2D(0, 1));
		if (CacheRockerX < 0)
		{
			Angle = -Angle;
		}
		float TargetYawTemp = NewControllerYaw + Angle;
		UKismetMathLibrary::FMod(TargetYawTemp, 360, TargetYaw);
		//同向
		float DifYaw = TargetYaw - SourceYaw;
		if (UKismetMathLibrary::Abs(DifYaw) > 180)
		{
			if (TargetYaw > 0)
			{
				TargetYaw = TargetYaw - 360;
			}
			else
			{
				TargetYaw = 360 + TargetYaw;
			}
		}
		//角度限制
		DifYaw = TargetYaw - SourceYaw;
		if (UKismetMathLibrary::Abs(DifYaw) > CacheMaxRotate)
		{
			if (DifYaw > 0)
			{
				TargetYaw = SourceYaw + CacheMaxRotate;
			}
			else
			{
				TargetYaw = SourceYaw - CacheMaxRotate;
			}
		}
	}
	//旋转
	InterpMovement->StartRotateToTagetYaw(TargetYaw, Time);
}

void ACSPlayer::CoverOperation(float NewRockerX, float NewRockerY, float NewMaxRotate, float NewSourceYaw, float NewControllerYaw, float NewTargetYaw)
{
	CacheRockerX = NewRockerX;
	CacheRockerY = NewRockerY;
	CacheMaxRotate = NewMaxRotate;
	CacheSourceYaw = NewSourceYaw;
	CacheControllerYaw = NewControllerYaw;
	CacheTargetYaw = NewTargetYaw;
}






