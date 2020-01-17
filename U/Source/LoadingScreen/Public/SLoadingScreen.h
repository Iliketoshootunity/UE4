// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "MoviePlayer.h"

typedef struct tagLoadingParam
{
	FString Name;
	float fMinPlayTime;
	bool  bMovice;
public:
	tagLoadingParam(FString ImageName = FString(TEXT("")), float MinPlayTime = 0,  bool bMovice = false)
		: Name(ImageName)
		, fMinPlayTime(MinPlayTime)
		, bMovice(bMovice)
	{
	}
}FLoadingParam;

DECLARE_DELEGATE_OneParam(FOnMoviePlaybackFinished,const int32 /*nMark*/)
/** Module interface for this game's loading screens */
class ILoadingScreenModule : public IModuleInterface 
{
private:
	FOnMoviePlaybackFinished m_OnMoviePlaybackFinishedDelegate;
public:
	/** Loads the module so it can be turned on */
	static inline ILoadingScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<ILoadingScreenModule>("LoadingScreen");
	}

	virtual void StartCarouselLoadingScreen(int32 nMark, bool bForcePlay,TArray<FLoadingParam> LoadingParamList) = 0;

	virtual void StartSingleLoadingScreen(int32 nMark, bool bForcePlay, FLoadingParam LoadingParam) = 0;

	/** Stops the loading screen */
	virtual void StopInGameLoadingScreen() = 0;

	virtual void RegistLoadingFinishedDelegate(FOnMoviePlaybackFinished InOnMoviePlaybackFinished)
	{
		m_OnMoviePlaybackFinishedDelegate = InOnMoviePlaybackFinished;
	}

	virtual FOnMoviePlaybackFinished& GetLoadingFinishedDelegate()
	{
		return	m_OnMoviePlaybackFinishedDelegate;
	}

};
