// Fill out your copyright notice in the Description page of Project Settings.


#include "SLoadingScreenWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "SGameViewportClient.h"
#include "BasicFunction.h"

ULoadingScreenWidget::ULoadingScreenWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_pLoadingTexture = nullptr;
	m_pLoadingProgressBar = nullptr;
	m_pDescText = nullptr;
	m_pProcessAnimation = nullptr;
}

bool ULoadingScreenWidget::Init()
{
	m_pLoadingTexture = GetWidget<UImage>(TEXT("LoadingTexture"));
	if (!m_pLoadingTexture)
	{
		return false;
	}

	m_pLoadingProgressBar = GetWidget<UProgressBar>(TEXT("LoadingProgressBar"));
	if (!m_pLoadingProgressBar)
	{
		return false;
	}

	m_pLoadingProgressBar->SetPercent(0.0f);

	m_pDescText = GetWidget<UTextBlock>(TEXT("Desc"));
	if (!m_pDescText)
	{
		return false;
	}

	m_pProcessAnimation = GetAnimation(TEXT("ProcessAnim"));
	if (!m_pProcessAnimation)
	{
		return false;
	}

	FScriptDelegate AnimationDynamicEvent;
	AnimationDynamicEvent.BindUFunction(this,TEXT("AnimationFinished"));
	BindToAnimationFinished(m_pProcessAnimation, FWidgetAnimationDynamicEvent(AnimationDynamicEvent));


	m_LoadMapFinished = false;

	return true;
}

void ULoadingScreenWidget::Refresh()
{

}

void ULoadingScreenWidget::BeginDestroy()
{
	Super::BeginDestroy();
}

void ULoadingScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsAnimationPlaying(m_pProcessAnimation))
		return;

	float fPercent = m_pLoadingProgressBar->Percent;
	if (fPercent >= 0.8f && !m_LoadMapFinished)
	{
		PauseAnimation(m_pProcessAnimation);
		return;
	}		
}

void ULoadingScreenWidget::Show(int32 nZOrder /*= 0*/, EUIType::Type eType /*= EUIType::UI_None*/)
{
	SetType(eType);

	if (IsVisible())
	{
		return;
	}

	USGameViewportClient::Get()->AddToViewport(this, nZOrder);

}

void ULoadingScreenWidget::Hide(bool bGC /*= true*/)
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

void ULoadingScreenWidget::SetProgressPercent(float fValue)
{
	if (!m_pLoadingProgressBar)
		return;

	m_pLoadingProgressBar->SetPercent(fValue);

}

void ULoadingScreenWidget::DoAnimToFinish(float fValue /*= 2.0f*/)
{
	if (!m_pLoadingProgressBar)
		return;

	m_LoadMapFinished = true;
	if (m_pProcessAnimation)
	{
		if (!IsAnimationPlaying(m_pProcessAnimation))
			PlayAnimationForward(m_pProcessAnimation, fValue);
		else
			SetPlaybackSpeed(m_pProcessAnimation, fValue);
	}
}

void ULoadingScreenWidget::SetAnimFinishedEvent(const FOnAnimFinished& event)
{
	m_OnANimFinished = event;
}

void ULoadingScreenWidget::SetTexture(UTexture2D* pTexture)
{
	if (!pTexture)
		return;

	if (m_pLoadingTexture)
		m_pLoadingTexture->SetBrushFromTexture(pTexture);
}

void ULoadingScreenWidget::PlayAnim()
{
	if(m_pProcessAnimation)
		PlayAnimationForward(m_pProcessAnimation);
}

void ULoadingScreenWidget::AnimationFinished()
{
	FBasicFunction::Logs(TEXT("AnimationFinished"));
	m_LoadMapFinished = false;

	m_OnANimFinished.ExecuteIfBound();

	m_OnANimFinished.Unbind();
}
