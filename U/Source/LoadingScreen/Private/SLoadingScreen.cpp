// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SLoadingScreen.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "SThrobber.h"

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!
struct FLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(NULL, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		if (UObject* CachedResourceObject = GetResourceObject())
		{
			Collector.AddReferencedObject(CachedResourceObject);
		}
	}
};

class SLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLoadingScreen)
		: _LoadingImageName(TEXT("/Game/UI/Loading/LoadingScreen.LoadingScreen"))
	{}
	SLATE_ATTRIBUTE(FName, LoadingImageName)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		LoadingImageName = InArgs._LoadingImageName.Get();

		LoadingScreenBrush = MakeShareable(new FLoadingScreenBrush(LoadingImageName, FVector2D(1920, 1080)));

		ChildSlot
			[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SSafeZone)
				.VAlign(VAlign_Bottom)
				.HAlign(HAlign_Right)
				.Padding(FMargin(10.0f))
				.IsTitleSafe(true)
				[
					SNew(SThrobber)
					.Visibility(this, &SLoadingScreen::GetLoadIndicatorVisibility)
				]
			]
		];
	}

private:
	/** Rather to show the ... indicator */
	EVisibility GetLoadIndicatorVisibility() const
	{
		//bool Vis =  GetMoviePlayer()->IsLoadingFinished();
		//return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
		return EVisibility::Visible;
	}
	
	/** Loading screen image brush */
	TSharedPtr<FLoadingScreenBrush> LoadingScreenBrush;

	FName LoadingImageName;

public:
	FString GetLoadingImageName()
	{
		return LoadingImageName.ToString();
	}
};


class LoadingScreenModule : public ILoadingScreenModule
{

private:
	bool								m_bWaitForMoviesToComplete;
	bool								m_bMoviesAreSkippable;
	int32								m_CurPlayingLoadingMark;
public:
	virtual void StartupModule() override
	{

		// Force load for cooker reference
		m_CurPlayingLoadingMark = INDEX_NONE;

		bool bWaitForMoviesToComplete = false;
		// Note: this code is executed too early so we cannot access UMoviePlayerSettings because the configs for that object have not been loaded and coalesced .  Have to read directly from the configs instead
		GConfig->GetBool(TEXT("/Script/MoviePlayer.MoviePlayerSettings"), TEXT("bWaitForMoviesToComplete"), bWaitForMoviesToComplete, GGameIni);
		GConfig->GetBool(TEXT("/Script/MoviePlayer.MoviePlayerSettings"), TEXT("bMoviesAreSkippable"), m_bMoviesAreSkippable, GGameIni);

		m_bWaitForMoviesToComplete = !bWaitForMoviesToComplete;

		GetMoviePlayer()->OnMoviePlaybackFinished().AddRaw(this, &LoadingScreenModule::OnMoviePlaybackFinished);

		if (IsMoviePlayerEnabled())
		{
			StartSingleLoadingScreen(1,true,FLoadingParam(TEXT("MovieLoading"),3.f,true));
		}
	}

	virtual void ShutdownModule() override
	{
		m_CurPlayingLoadingMark = INDEX_NONE;
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}

	void		OnMoviePlaybackFinished()
	{
		if (m_CurPlayingLoadingMark != INDEX_NONE)
		{
			GetLoadingFinishedDelegate().ExecuteIfBound(m_CurPlayingLoadingMark);
			m_CurPlayingLoadingMark = INDEX_NONE;
		}
	}

	virtual void StartCarouselLoadingScreen(int32 nMark, bool bForcePlay, TArray<FLoadingParam> LoadingParamList) override
	{
		if (GetMoviePlayer()->IsStartupMoviePlaying() && bForcePlay && LoadingParamList.Num() > 0)
		{
			GetMoviePlayer()->StopMovie();
		}

		TArray<FLoadingParam> _LoadingParamList = LoadingParamList;
		while(_LoadingParamList.Num() > 0)
		{
			FLoadingParam LoadingParam = _LoadingParamList.Pop();
			
			FString AssetName;
			FLoadingScreenAttributes LoadingScreen;
			if (LoadingParam.bMovice)
			{
				AssetName = LoadingParam.Name.IsEmpty() ? TEXT("MovieLoading") : LoadingParam.Name;

				LoadingScreen.MinimumLoadingScreenDisplayTime = LoadingParam.fMinPlayTime;
				LoadingScreen.bAutoCompleteWhenLoadingCompletes = m_bWaitForMoviesToComplete;
				LoadingScreen.bMoviesAreSkippable = m_bMoviesAreSkippable;
				LoadingScreen.MoviePaths.Add(AssetName);

			}
			else
			{
				AssetName = LoadingParam.Name.IsEmpty() ? TEXT("/Game/UI/Loading/LoadingScreen.LoadingScreen") : LoadingParam.Name;

				LoadObject<UObject>(NULL, *AssetName);

				LoadingScreen.MinimumLoadingScreenDisplayTime = LoadingParam.fMinPlayTime;
				LoadingScreen.bAutoCompleteWhenLoadingCompletes = m_bWaitForMoviesToComplete;
				LoadingScreen.bMoviesAreSkippable = m_bMoviesAreSkippable;
				LoadingScreen.WidgetLoadingScreen = SNew(SLoadingScreen)
					.LoadingImageName(*AssetName);
			}

			if (!LoadingScreen.IsValid())
			{
				return;
			}
			
			GetMoviePlayer()->WaitForMovieToFinish();

			if(!GetMoviePlayer()->IsLoadingFinished() && GetMoviePlayer()->IsMovieCurrentlyPlaying())
				GetMoviePlayer()->WaitForMovieToFinish();

			GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
			GetMoviePlayer()->PlayMovie();
			if (_LoadingParamList.Num() <= 0)
			{
				m_CurPlayingLoadingMark = nMark;
			}

		}
	}

	virtual void StartSingleLoadingScreen(int32 nMark, bool bForcePlay, FLoadingParam LoadingParam)override
	{
		FString AssetName;
		FLoadingScreenAttributes LoadingScreen;
		if (LoadingParam.bMovice)
		{
			AssetName = LoadingParam.Name.IsEmpty() ? TEXT("MovieLoading") : LoadingParam.Name;

			LoadingScreen.MinimumLoadingScreenDisplayTime = LoadingParam.fMinPlayTime;
			LoadingScreen.bAutoCompleteWhenLoadingCompletes = m_bWaitForMoviesToComplete;
			LoadingScreen.bMoviesAreSkippable = m_bMoviesAreSkippable;
			LoadingScreen.MoviePaths.Add(AssetName);

		}
		else
		{
			AssetName = LoadingParam.Name.IsEmpty() ? TEXT("/Game/UI/Loading/LoadingScreen.LoadingScreen") : LoadingParam.Name;

			LoadObject<UObject>(NULL, *AssetName);

			LoadingScreen.MinimumLoadingScreenDisplayTime = LoadingParam.fMinPlayTime;
			LoadingScreen.bAutoCompleteWhenLoadingCompletes = m_bWaitForMoviesToComplete;
			LoadingScreen.bMoviesAreSkippable = m_bMoviesAreSkippable;
			LoadingScreen.WidgetLoadingScreen = SNew(SLoadingScreen)
				.LoadingImageName(*AssetName);
		}

		if (!LoadingScreen.IsValid())
		{
			return;
		}

		if (GetMoviePlayer()->IsStartupMoviePlaying() && bForcePlay)
		{
			GetMoviePlayer()->StopMovie();
		}

		GetMoviePlayer()->WaitForMovieToFinish();
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		GetMoviePlayer()->PlayMovie();

		m_CurPlayingLoadingMark = nMark;
	}


	virtual void StopInGameLoadingScreen() override
	{
		GetMoviePlayer()->StopMovie();
	}
};

IMPLEMENT_GAME_MODULE(LoadingScreenModule, LoadingScreen);

