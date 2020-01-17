// Fill out your copyright notice in the Description page of Project Settings.


#include "SLoadingManager.h"
#include "SLoadingScreen.h"
#include "OtherSystem/Pregame/SPregameManager.h"
#include "SLoadingScreenTable.h"
#include "WorldSystem/SWorldManager.h"
#include "Kismet/GameplayStatics.h"

ULoadingManager::ULoadingManager()
{
	Init();
}

void ULoadingManager::Init()
{
	//ILoadingScreenModule::Get().RegistLoadingFinishedDelegate(FOnMoviePlaybackFinished::CreateUObject(this, &ULoadingManager::OnLoadingFinished));
}

void ULoadingManager::ShowLoading(ELoadingMark::Type eMark, int32 nLoadingScreenID)
{
	StopLoading();

	bool bForcePlay = SLoadingScreenTable::Get()->IsForcePlay(nLoadingScreenID);
	TArray<FLoadingAssetData> LoadingAssetData = SLoadingScreenTable::Get()->GetLoadingAsset(nLoadingScreenID);

	TArray<FLoadingParam> LoadingParamList;
	for (FLoadingAssetData data : LoadingAssetData)
	{
		FLoadingParam Param;
		Param.Name = data.AssetName;
		Param.fMinPlayTime = data.MiniDisplayTime;
		Param.bMovice = data.IsMovice;
		LoadingParamList.Insert(Param, 0);
	}

	ILoadingScreenModule::Get().StartCarouselLoadingScreen(eMark, bForcePlay, LoadingParamList);
}

void ULoadingManager::StopLoading()
{
	ILoadingScreenModule::Get().StopInGameLoadingScreen();
}

void ULoadingManager::OnLoadingFinished(const int32 nMark)
{

	UWorld* pCurWorld = GetWorld();

	switch (nMark)
	{
	case ELoadingMark::Mark_Launcher:
	{
		UPregameManager::Get()->ShowLauncher();
	}
	break;
	case ELoadingMark::Mark_ChangeMap:
	{
		
	}
	break;
	default:
		break;
	}

	
}
