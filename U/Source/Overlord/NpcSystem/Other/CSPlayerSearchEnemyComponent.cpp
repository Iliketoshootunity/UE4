// Fill out your copyright notice in the Description page of Project Settings.


#include "CSPlayerSearchEnemyComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BaseSystem/Ext/MathfExtLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Character/CSCharacter.h"
#include "../Character/CSPlayer.h"

void UCSPlayerSearchEnemyComponent::SetUpParameters(float InLockRange, float InFirsetLock, float InNoRockerAngleLimit, float InHasRockerAngleLimit, float InMaxRotateAngle, FVector2D InRockerInput)
{
	LockRange = InLockRange;
	FirsetLock = InFirsetLock;
	NoRockerAngleLimit = InHasRockerAngleLimit;
	HasRockerAngleLimit = InHasRockerAngleLimit;
	MaxRotateAngle = InMaxRotateAngle;
	RockerInput = InRockerInput;
}

AActor* UCSPlayerSearchEnemyComponent::ExecuteSearchEnemy()
{
	Reset();

	AActor* SearchEnemyTemp = nullptr;

	if (RockerInput.X == 0 && RockerInput.Y == 0)
	{
		SearchEnemyTemp = SearchEnemysNoHasRocker();
	}
	else
	{
		SearchEnemyTemp = SearchEnemysWhenHasRocker();
	}

	SearchEnemy = SearchEnemyTemp;
	if (SearchEnemy)
	{
		//计算目标旋转
		TargetPosition = SearchEnemy->GetActorLocation();
		FVector StartPostion = GetOwner()->GetActorLocation();
		float SourceYaw = GetOwner()->GetActorRotation().Yaw;
		FRotator TargetRotate = UKismetMathLibrary::FindLookAtRotation(StartPostion, TargetPosition);
		float TargetYaw = TargetRotate.Yaw;
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
		if (UKismetMathLibrary::Abs(DifYaw) > MaxRotateAngle)
		{
			if (DifYaw > 0)
			{
				TargetYaw = SourceYaw + MaxRotateAngle;
			}
			else
			{
				TargetYaw = SourceYaw - MaxRotateAngle;
			}
		}
		TargetDirection = FRotator(0, TargetYaw, 0);

	}
	return SearchEnemy;
}

void UCSPlayerSearchEnemyComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* OwnerTemp = GetOwner();
	if (OwnerTemp)
	{
		Character = Cast<ACSCharacter>(OwnerTemp);
	}
}

AActor* UCSPlayerSearchEnemyComponent::SearchEnemysWhenHasRocker()
{
	TArray<AActor*> LockRangeSphereTraceEnemys = SearchEnemysBySphereTrace(LockRange);
	TArray<AActor*> MatchConditonsEnemys;
	if (LockRangeSphereTraceEnemys.Num() <= 0)return nullptr;
	FRotator ControllerRotate = Character->GetControlRotation();
	FVector RotateForward = UKismetMathLibrary::GetForwardVector(ControllerRotate);
	FVector YawForward = FVector(RotateForward.X, RotateForward.Y, 0);
	float Angle = UMathfExtLibrary::VectorAngle2D(FVector2D(0, 1), RockerInput);
	if (RockerInput.X < 0)
	{
		Angle = -Angle;
	}
	FRotator RockerRotate = FRotator(0, Angle, 0);
	FVector NewYawForward = RockerRotate.RotateVector(YawForward);
	FVector2D NewYawForward2D = FVector2D(NewYawForward.X, NewYawForward.Y);
	for (size_t i = 0; i < LockRangeSphereTraceEnemys.Num(); i++)
	{
		FVector EnemyLocation = LockRangeSphereTraceEnemys[i]->GetActorLocation();
		FVector RelativePosition = EnemyLocation - GetOwner()->GetActorLocation();
		float Angle = UMathfExtLibrary::VectorAngle2D(NewYawForward2D, FVector2D(RelativePosition.X, RelativePosition.Y));
		if (Angle < HasRockerAngleLimit)
		{
			MatchConditonsEnemys.Add(LockRangeSphereTraceEnemys[i]);
		}
	}

	if (MatchConditonsEnemys.Num() <= 0)return nullptr;
	return SelectEnemy_MinDiistance(MatchConditonsEnemys);
}

AActor* UCSPlayerSearchEnemyComponent::SearchEnemysNoHasRocker()
{
	TArray<AActor*> LockRangeSphereTraceEnemys = SearchEnemysBySphereTrace(LockRange);
	if (LockRangeSphereTraceEnemys.Num() <= 0)return nullptr;
	TArray<AActor*> InAngleEnemys;
	FRotator ControllerRotate = Character->GetControlRotation();
	FVector RotateForward = UKismetMathLibrary::GetForwardVector(ControllerRotate);
	FVector2D YawForward = FVector2D(RotateForward.X, RotateForward.Y);
	for (size_t i = 0; i < LockRangeSphereTraceEnemys.Num(); i++)
	{
		FVector EnemyLocation = LockRangeSphereTraceEnemys[i]->GetActorLocation();
		FVector RelativePosition = EnemyLocation - GetOwner()->GetActorLocation();
		float Angle = UMathfExtLibrary::VectorAngle2D(YawForward, FVector2D(RelativePosition.X, RelativePosition.Y));
		if (Angle < NoRockerAngleLimit)
		{
			InAngleEnemys.Add(LockRangeSphereTraceEnemys[i]);
		}
	}
	TArray<AActor*> FirstRangeSphereTraceEnemys = SearchEnemysBySphereTrace(FirsetLock);
	TArray<AActor*> MatchAllContionsEnemy;
	if (FirstRangeSphereTraceEnemys.Num() > 0)
	{
		for (size_t i = 0; i < InAngleEnemys.Num(); i++)
		{
			for (size_t j = 0; j < FirstRangeSphereTraceEnemys.Num(); j++)
			{
				if (InAngleEnemys[i] == FirstRangeSphereTraceEnemys[j])
				{
					MatchAllContionsEnemy.Add(InAngleEnemys[i]);
				}
			}
		}

		if (MatchAllContionsEnemy.Num() > 0)
		{
			return SelectEnemy_MinDiistance(MatchAllContionsEnemy);
		}
		else
		{
			return SelectEnemy_MinDiistance(FirstRangeSphereTraceEnemys);
		}
	}
	else
	{
		return SelectEnemy_MinDiistance(InAngleEnemys);
	}

}

TArray<AActor*> UCSPlayerSearchEnemyComponent::SearchEnemysBySphereTrace(float Range)
{
	TArray<AActor*> Enemys;
	UWorld* CurWorld = GetWorld();
	if (CurWorld == nullptr)
		return Enemys;
	////搜寻锁定距离内敌人
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	TArray<FHitResult> OutHits;
	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::ForDuration;
	if (DrawTime <= 0)
	{
		DebugType = EDrawDebugTrace::Type::None;
	}
	UKismetSystemLibrary::SphereTraceMultiForObjects((UObject*)CurWorld,
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation() + FVector(1, 1, 1),
		Range, ObjectTypes, false,
		ActorsToIgnore, DebugType, OutHits, true, FLinearColor::Red, FLinearColor::Blue, DrawTime);
	for (size_t i = 0; i < OutHits.Num(); i++)
	{
		AActor* Actor = OutHits[i].GetActor();
		ACSPlayer* Player = Cast<ACSPlayer>(Actor);
		if (Player == nullptr)
		{
			Enemys.Add(OutHits[i].GetActor());
		}
	}
	return Enemys;

}

AActor* UCSPlayerSearchEnemyComponent::SelectEnemy_MinDiistance(TArray<AActor*> Enemys)
{
	if (Enemys.Num() < 0)return nullptr;
	AActor* MinDisEnemy = nullptr;
	float MinDis = 100000000000;
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	for (size_t i = 0; i < Enemys.Num(); i++)
	{
		FVector EnemyLocation = Enemys[i]->GetActorLocation();
		float Distance = UKismetMathLibrary::Vector_Distance(OwnerLocation, EnemyLocation);
		if (Distance < MinDis)
		{
			MinDis = Distance;
			MinDisEnemy = Enemys[i];
		}
	}
	return MinDisEnemy;

}
