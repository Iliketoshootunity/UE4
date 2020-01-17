// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSSearchEnemyComponent.h"
#include "CSPlayerSearchEnemyComponent.generated.h"

class AActor;
class ACSCharacter;


/**
 *
 */
UCLASS(Blueprintable)
class OVERLORD_API UCSPlayerSearchEnemyComponent : public UCSSearchEnemyComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SearchEnemyParameter) float LockRange = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SearchEnemyParameter) float FirsetLock = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SearchEnemyParameter) float HasRockerAngleLimit = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SearchEnemyParameter) float NoRockerAngleLimit = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SearchEnemyParameter) float MaxRotateAngle = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SearchEnemyParameter) FVector2D RockerInput = FVector2D::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug) float DrawTime = 5;

	UPROPERTY() ACSCharacter* Character;

public:
	UFUNCTION(BlueprintCallable, Category = SearchEnemy)
		void SetUpParameters(float InLockRange, float InFirsetLock, float InNoRockerAngleLimit, float InHasRockerAngleLimit, float InMaxRotateAngle, FVector2D InRockerInput);

	//UFUNCTION(BlueprintCallable,Category=SearchEnemy)
	virtual AActor* ExecuteSearchEnemy() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	AActor* SearchEnemysWhenHasRocker();
	AActor* SearchEnemysNoHasRocker();
	TArray<AActor*> SearchEnemysBySphereTrace(float Range);
	AActor* SelectEnemy_MinDiistance(TArray<AActor*> Enemys);

};
