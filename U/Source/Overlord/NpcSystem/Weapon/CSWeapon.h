// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSWeapon.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class ACSCharacter;

UCLASS()
class OVERLORD_API ACSWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere) UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere) UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere) ACSCharacter* Character;
public:
	UFUNCTION(BlueprintImplementableEvent, Category = Move) void Show(float Duration);
	UFUNCTION(BlueprintImplementableEvent, Category = Move) void Hide(float Duration);
	void ActiveBox();
	void DeactiveBox();
	UFUNCTION(BlueprintCallable) void SetCharacter(ACSCharacter* InCharacter);
	UFUNCTION(BlueprintPure) ACSCharacter* GetCharacter();
};
