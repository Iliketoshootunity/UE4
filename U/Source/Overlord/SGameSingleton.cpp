// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameSingleton.h"
#include "Engine/Engine.h"
#include "BaseSystem/SingletonManager.h"
#include "OtherSystem/Pregame/SPregameManager.h"
#include "OtherSystem/Loading/SLoadingManager.h"
#include "UISystem/SWidgetManager.h"
#include "TableManager.h"
#include "WorldSystem/SWorldManager.h"

UGameSingleton::UGameSingleton()
{
}

UGameSingleton* UGameSingleton::Get()
{
	UGameSingleton* pSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);

	if (!pSingleton || !pSingleton->IsValidLowLevel())
	{
		GEngine->GameSingleton = NewObject<UGameSingleton>();
		pSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
	}

	return pSingleton;
}

void UGameSingleton::Init()
{

	FSingletonManager::Get()->AddSingleton<ULoadingManager>();
	FSingletonManager::Get()->AddSingleton<UPregameManager>();
	FSingletonManager::Get()->AddSingleton<UWorldManager>();

	TableManager::Get()->UnLoad();
	TableManager::Get()->Load();
	FWidgetManager::Get()->Init();
	UWorldManager::Get()->Init();
}

void UGameSingleton::StartGame()
{
	FWidgetManager::Get()->StartGame();
	//UWorldManager::Get()->LoadMap(1,false);
}

void UGameSingleton::BeginDestroy()
{
	TableManager::Get()->UnLoad();
	FWidgetManager::Get()->Shutdown();
	Super::BeginDestroy();
}

void UGameSingleton::Tick(float fDeltaTime)
{
	UPregameManager::Get()->Tick(fDeltaTime);
}
