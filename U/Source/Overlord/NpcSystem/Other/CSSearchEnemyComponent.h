// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSSearchEnemyComponent.generated.h"

class Actor;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class OVERLORD_API UCSSearchEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	//目标位置
	FVector TargetPosition;
	//目标方向
	FRotator TargetDirection;
	//目标敌人
	UPROPERTY()
	AActor* SearchEnemy;

public:
	// Sets default values for this component's properties
	UCSSearchEnemyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:


public:
	UFUNCTION(BlueprintCallable,Category=SearchEnemy)
	virtual AActor* ExecuteSearchEnemy();
	UFUNCTION(BlueprintCallable, Category = SearchEnemy)
	virtual void Reset();

	UFUNCTION(BlueprintPure) FVector GetTargetPosition();
	UFUNCTION(BlueprintPure) FRotator GetTargetDirection();

	UFUNCTION(BlueprintCallable) void SetTargetPosition(FVector NewPos);
	UFUNCTION(BlueprintCallable) void SetTargetDirection(FRotator NewRot);
	UFUNCTION(BlueprintCallable) void SetSearchEnemy(AActor* NewSearchedEnemy);
};
