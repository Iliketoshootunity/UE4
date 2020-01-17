// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameViewportClient.h"
#include "UISystem/SBaseWidget.h"
#include "SGameInstance.h"
#include "Engine/Engine.h"
#include "TimerManager.h"



USGameViewportClient::USGameViewportClient(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_TDelayAddToViewportMap.Empty();
	m_TDelayRemoveViewportMap.Empty();
	m_CurrentWidget = nullptr;
	SetSuppressTransitionMessage(true);
}

void USGameViewportClient::AddViewportWidgetContent(TSharedRef<class SWidget> ViewportContent, const int32 ZOrder /*= 0*/)
{
	Super::AddViewportWidgetContent(ViewportContent, ZOrder);
}

void USGameViewportClient::RemoveViewportWidgetContent(TSharedRef<class SWidget> ViewportContent)
{
	m_CurrentWidget = nullptr;
	Super::RemoveViewportWidgetContent(ViewportContent);
}

void USGameViewportClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void USGameViewportClient::BeginDestroy()
{
	if(g_pGameInstance)
		g_pGameInstance->GetTimerManager().ClearAllTimersForObject(this);

	m_TDelayRemoveViewportMap.Empty();

	m_TDelayAddToViewportMap.Empty();

	m_CurrentWidget = nullptr;

	Super::BeginDestroy();
}

USGameViewportClient* USGameViewportClient::Get()
{
	if (g_pGameInstance == nullptr || (g_pGameInstance && !g_pGameInstance->IsValidLowLevel()))
	{
		return Cast<USGameViewportClient>(GEngine->GameViewport);;
	}

	return Cast<USGameViewportClient>(g_pGameInstance->GetGameViewportClient());
}

void USGameViewportClient::AddToViewport(class UBaseWidget* pWidget, const int32 ZOrder /*= 0*/, float fDelayTime /*= 0.0f*/)
{
	if (pWidget == nullptr || (pWidget && !pWidget->IsValidLowLevel()))
	{
		return;
	}

	//已经显示
	if (pWidget->IsVisible())
	{
		return;
	}
	//如果已经在延迟显示列表里
	if (m_TDelayAddToViewportMap.Contains(pWidget))
	{
		return;
	}

	if (fDelayTime <= 0.0f)
	{
		AddViewportWidgetContent(pWidget->TakeWidget(), ZOrder);
		m_CurrentWidget = pWidget;
	}
	else
	{
		UObject* pObject = Cast<UObject>(pWidget);
		if (m_TDelayAddToViewportMap.Contains(pObject))
		{
			return;
		}

		FTimerHandle TimerHandle;
		FTimerDelegate TimeDelegate = FTimerDelegate::CreateUObject(this, &USGameViewportClient::DelayAddToViewport, pObject, ZOrder);
		g_pGameInstance->GetTimerManager().SetTimer(TimerHandle, TimeDelegate, fDelayTime, false);
		m_TDelayAddToViewportMap.Add(pObject, TimerHandle);
	}
}

void USGameViewportClient::DelayAddToViewport(UObject* pObject, const int32 ZOrder)
{
	if (pObject == nullptr || (pObject && !pObject->IsValidLowLevel()))
	{
		return;
	}

	UBaseWidget* pBaseWidget = Cast<UBaseWidget>(pObject);

	if (pBaseWidget == nullptr || (pBaseWidget && !pBaseWidget->IsValidLowLevel()))
	{
		return;
	}


	AddViewportWidgetContent(pBaseWidget->TakeWidget(), ZOrder);

	FTimerHandle TimerHandle = m_TDelayAddToViewportMap.FindRef(pObject);

	g_pGameInstance->GetTimerManager().ClearTimer(TimerHandle);

	m_TDelayAddToViewportMap.Remove(pObject);

	m_CurrentWidget = pBaseWidget;
}



void USGameViewportClient::RemoveFromViewport(class UBaseWidget* pWidget, float fDelayTime /*= 0.0f*/)
{
	if (pWidget == nullptr)
	{
		return;
	}

	
	if (fDelayTime <= 0.0f)
	{
		RemoveViewportWidgetContent(pWidget->TakeWidget());
		pWidget->SetGC(true);
	}
	else
	{
		UObject* pObject = Cast<UObject>(pWidget);
		if (m_TDelayRemoveViewportMap.Contains(pObject))
		{
			return;
		}

		FTimerHandle TimerHandle;
		FTimerDelegate TimeDelegate = FTimerDelegate::CreateUObject(this, &USGameViewportClient::DelayRemoveFromViewport, pObject);
		g_pGameInstance->GetTimerManager().SetTimer(TimerHandle, TimeDelegate, fDelayTime, false);
		m_TDelayRemoveViewportMap.Add(pObject, TimerHandle);
	}
}

void USGameViewportClient::DelayRemoveFromViewport(UObject* pWidget)
{
	if (pWidget == nullptr || (pWidget && !pWidget->IsValidLowLevel()))
	{
		return;
	}

	UBaseWidget* pBaseWidget = Cast<UBaseWidget>(pWidget);

	if (pBaseWidget == nullptr || (pBaseWidget && !pBaseWidget->IsValidLowLevel()))
	{
		return;
	}


	RemoveViewportWidgetContent(pBaseWidget->TakeWidget());

	FTimerHandle TimerHandle = m_TDelayRemoveViewportMap.FindRef(pWidget);

	g_pGameInstance->GetTimerManager().ClearTimer(TimerHandle);

	m_TDelayRemoveViewportMap.Remove(pWidget);

	pBaseWidget->SetGC(true);
}
