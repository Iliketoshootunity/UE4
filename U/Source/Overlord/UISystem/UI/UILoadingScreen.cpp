// Fill out your copyright notice in the Description page of Project Settings.


#include "UILoadingScreen.h"
#include "ProgressBar.h"
#include "SGameViewportClient.h"
#include "TextBlock.h"
#include "Image.h"

UUILoadingScreen::UUILoadingScreen(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LoadingProgressBar = nullptr;
	DescText = nullptr;
	LoadingTexture = nullptr;
}

bool UUILoadingScreen::Init()
{
	LoadingProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("LoadingProgress")));
	DescText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DescText")));
	LoadingTexture = Cast<UImage>(GetWidgetFromName(TEXT("LoadingTexture")));
	ProcessAnimation = GetAnimation(TEXT("ProcessAnimation"));

	return true;
}

void UUILoadingScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bExcuteSimulate)
	{
		SimulateTimer += InDeltaTime;
		float P = SimulateTimer / SimulateTime;
		if (P >= 1)
		{
			P = 1;
		}
		SetProgressPercent(SimulateStart + (SimulateEnd - SimulateStart) * P);
		if (P == 1)
		{
			if (bLoadMapFinished)
			{
				OnAnimFinished.ExecuteIfBound();
				OnAnimFinished.Unbind();
			}
			bExcuteSimulate = false;
		}
	}
}

void UUILoadingScreen::Show(int32 nZOrder /*= 0*/, EUIType::Type eType /*= EUIType::UI_None*/)
{
	SetType(eType);

	if (IsVisible())
	{
		return;
	}
	bExcuteSimulate = false;
	bLoadMapFinished = false;
	USGameViewportClient::Get()->AddToViewport(this, nZOrder);
}

void UUILoadingScreen::Hide(bool bGC /*= true*/)
{
	//如果UI没有显示，则返回
	if (!IsVisible())
	{
		return;
	}

	USGameViewportClient::Get()->RemoveFromViewport(this);

	if (IsRooted())
	{
		RemoveFromRoot();
	}

	SetGC(bGC);
}

void UUILoadingScreen::SetProgressPercent(float Value)
{
	LoadingProgressBar->SetPercent(Value);
}


void UUILoadingScreen::SetAnimFinishedEvent(const FOnAnimFinished& event)
{
	OnAnimFinished = event;
}


void UUILoadingScreen::OnLoadMapStart()
{
	//模拟到80%
	SimulateStart = LoadingProgressBar->Percent;
	SimulateEnd = 0.8f;
	SimulateTime = 3.f;
	SimulateTimer = 0;
	bExcuteSimulate = true;
}

void UUILoadingScreen::OnLoadMapFinshed()
{
	float LoadingProgress = LoadingProgressBar->Percent;
	if (LoadingProgress >= 1.0f)
	{
		bExcuteSimulate = false;
		OnAnimFinished.ExecuteIfBound();
		OnAnimFinished.Unbind();
		return;
	}
	bLoadMapFinished = true;
	SimulateTimer = 0;
	SimulateTime = 0.5f;
	SimulateStart = LoadingProgress;
	SimulateEnd = 1.f;
	bExcuteSimulate = true;
}

