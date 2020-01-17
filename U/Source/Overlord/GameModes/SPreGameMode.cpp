// Fill out your copyright notice in the Description page of Project Settings.


#include "SPreGameMode.h"
#include "SGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void ASPreGameMode::StartPlay()
{
	Super::StartPlay();

	APlayerController* pCharacterController = UGameplayStatics::GetPlayerController(g_pGameInstance->GetWorld(), 0);
	if (pCharacterController)
	{
		pCharacterController->SetVirtualJoystickVisibility(false);
		pCharacterController->bShowMouseCursor = true;
		pCharacterController->SetCinematicMode(true, true, true, true, true);
	}
}

void ASPreGameMode::BeginDestroy()
{
	if (g_pGameInstance)
	{
		APlayerController* pCharacterController = UGameplayStatics::GetPlayerController(g_pGameInstance->GetWorld(), 0);
		if (pCharacterController)
		{
			pCharacterController->SetVirtualJoystickVisibility(true);
			pCharacterController->bShowMouseCursor = false;
			pCharacterController->SetCinematicMode(true, false, false, false, false);
		}
	}

	Super::BeginDestroy();
}
