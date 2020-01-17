// Fill out your copyright notice in the Description page of Project Settings.


#include "CSSearchEnemyComponent.h"

// Sets default values for this component's properties
UCSSearchEnemyComponent::UCSSearchEnemyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UCSSearchEnemyComponent::BeginPlay()
{
	Super::BeginPlay();
}


AActor* UCSSearchEnemyComponent::ExecuteSearchEnemy()
{
	return nullptr;
}

void UCSSearchEnemyComponent::Reset()
{
	SearchEnemy = nullptr;
	TargetPosition = FVector::ZeroVector;
	TargetDirection = FRotator::ZeroRotator;
}

FVector UCSSearchEnemyComponent::GetTargetPosition()
{
	return TargetPosition;
}

UFUNCTION(BlueprintPure)FRotator UCSSearchEnemyComponent::GetTargetDirection()
{
	return TargetDirection;
}

void UCSSearchEnemyComponent::SetTargetPosition(FVector NewPos)
{
	TargetPosition = NewPos;
}

void UCSSearchEnemyComponent::SetTargetDirection(FRotator NewRot)
{
	TargetDirection = NewRot;
}

void UCSSearchEnemyComponent::SetSearchEnemy(AActor* NewSearchEnemy)
{
	SearchEnemy = NewSearchEnemy;
}


