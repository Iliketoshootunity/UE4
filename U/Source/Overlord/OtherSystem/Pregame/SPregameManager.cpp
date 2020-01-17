// Fill out your copyright notice in the Description page of Project Settings.


#include "SPregameManager.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SGameInstance.h"
#include "Engine/LevelStreaming.h"
#include "UISystem/SWidgetManager.h"
#include "UISystem/UI/SRoleWidget.h"
#include "WorldSystem/SWorldManager.h"

void UPregameManager::BeginDestroy()
{
	Super::BeginDestroy();
}

void UPregameManager::ShowLauncher()
{
	//FLatentActionInfo LatentInfo;
	//UGameplayStatics::LoadStreamLevel(g_pGameInstance->GetWorld(), TEXT("Launch"), true, false, LatentInfo);

	FWidgetManager::Get()->Show(EUIType::UI_Login);
	m_Status = 1;
	
}

void UPregameManager::ShowLogin()
{
	ULevelStreaming* pLaunchStreamLevel = UGameplayStatics::GetStreamingLevel(g_pGameInstance->GetWorld(), TEXT("CreateRole"));
	if (pLaunchStreamLevel && pLaunchStreamLevel->IsLevelVisible())
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.Linkage = EStreamLevelState::CreateRole;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = TEXT("UnloadSLFinished");
		UGameplayStatics::UnloadStreamLevel(g_pGameInstance->GetWorld(), TEXT("CreateRole"), LatentInfo, false);
	}

	FWidgetManager::Get()->Hide(EUIType::UI_CSRole);
	FWidgetManager::Get()->Hide(EUIType::UI_Launcher);
	//FWidgetManager::Get()->Show(EUIType::UI_Login);
}

void UPregameManager::ShowCreateRole()
{
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = EStreamLevelState::Launcher;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = TEXT("UnloadSLFinished");
	UGameplayStatics::UnloadStreamLevel(g_pGameInstance->GetWorld(), TEXT("Launch"), LatentInfo, false);

	FWidgetManager::Get()->Hide(EUIType::UI_Login);
	URoleWidget* pWidget = Cast<URoleWidget>(FWidgetManager::Get()->Show(EUIType::UI_CSRole));
	if (!pWidget) return;

	pWidget->ShowCreateRoleWidget();
}

void UPregameManager::ShowSelectRole()
{
	if (FWidgetManager::Get()->IsVisible(EUIType::UI_Login))
		FWidgetManager::Get()->Hide(EUIType::UI_Login);

	URoleWidget* pWidget = Cast<URoleWidget>(FWidgetManager::Get()->Show(EUIType::UI_CSRole));
	if (!pWidget) return;

	pWidget->ShowSelectRoleWidget();
}

void UPregameManager::Tick(float fDeltaTime)
{
	if (m_Status == 1)
	{
		m_Status = 0;
		
	}
}

void UPregameManager::UnloadSLFinished(int32 LinkID)
{
	switch (LinkID)
	{
	case EStreamLevelState::CreateRole:
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.Linkage = EStreamLevelState::Launcher;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = TEXT("LoadSLFinished");
		UGameplayStatics::LoadStreamLevel(g_pGameInstance->GetWorld(), TEXT("Launch"), true, false, LatentInfo);
	}
	break;
	case EStreamLevelState::Launcher:
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.Linkage = EStreamLevelState::CreateRole;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = TEXT("LoadSLFinished");
		UGameplayStatics::LoadStreamLevel(g_pGameInstance->GetWorld(), TEXT("CreateRole"), true, false, LatentInfo);
	}
	break;
	default:
		break;
	}
}

void UPregameManager::LoadSLFinished(int32 LinkID)
{
	switch (LinkID)
	{
	case EStreamLevelState::CreateRole:
	{

	}
	break;
	case EStreamLevelState::Launcher:
	{

	}
	break;
	default:
		break;
	}
}
