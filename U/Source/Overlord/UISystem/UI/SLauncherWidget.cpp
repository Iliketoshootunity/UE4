// Fill out your copyright notice in the Description page of Project Settings.


#include "SLauncherWidget.h"
#include "OtherSystem/Pregame/SPregameManager.h"
#include "TextBlock.h"
#include "SGameViewportClient.h"

ULauncherWidget::ULauncherWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool ULauncherWidget::Init()
{
	m_pAnimation = GetAnimation(TEXT("LauncherAnim"));
	if (m_pAnimation == nullptr) return false;

	m_pText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Desc")));
	if (!m_pText) return false;

	PlayAnimation(m_pAnimation);
	return true;
}

void ULauncherWidget::Refresh()
{

}

void ULauncherWidget::Show(int32 nZOrder /*= 0*/, EUIType::Type eType /*= EUIType::UI_None*/)
{
	SetType(eType);

	if (IsVisible())
	{
		return;
	}

	USGameViewportClient::Get()->AddToViewport(this, nZOrder);
}

void ULauncherWidget::Hide(bool bGC /*= true*/)
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

void ULauncherWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	UPregameManager::Get()->ShowLogin();
}