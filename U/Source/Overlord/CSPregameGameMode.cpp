// Fill out your copyright notice in the Description page of Project Settings.


#include "CSPregameGameMode.h"
#include "UISystem/SWidgetManager.h"
#include "MessageDispatcher.h"

void ACSPregameGameMode::BeginPlay()
{
	Super::BeginPlay();
	FWidgetManager::Get()->Show(EUIType::UI_Login);
}

