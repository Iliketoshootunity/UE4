// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeapon.h"
#include "Components/BoxComponent.h"
#include "../Character/CSCharacter.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACSWeapon::ACSWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACSWeapon::BeginPlay()
{
	Super::BeginPlay();

	//获取组件
	UClass* MeshClass = UStaticMeshComponent::StaticClass();
	Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(MeshClass));

	UClass* BoxClass = UBoxComponent::StaticClass();
	BoxComponent = Cast<UBoxComponent>(GetComponentByClass(BoxClass));

	//Hide();
	DeactiveBox();
}

// Called every frame
void ACSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACSWeapon::ActiveBox()
{
	if (BoxComponent)
	{
		BoxComponent->SetGenerateOverlapEvents(true);
	}
}

void ACSWeapon::DeactiveBox()
{
	if (BoxComponent)
	{
		BoxComponent->SetGenerateOverlapEvents(false);
	}
}
void ACSWeapon::SetCharacter(ACSCharacter* InCharacter)
{
	Character = InCharacter;
}

ACSCharacter* ACSWeapon::GetCharacter()
{
	return Character;
}


