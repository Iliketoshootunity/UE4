// Fill out your copyright notice in the Description page of Project Settings.


#include "SWidgetManager.h"
#include "BaseConfig.h"
#include "BaseSystem/GameUtil.h"
#include "SGameInstance.h"
#include "SGameViewportClient.h"


void FWidgetManager::Init()
{
	m_TAssertMap.Empty();
	m_TWidgetMap.Empty();

	m_TAssertMap.Add(EUIType::UI_LoadingScreen, SBaseConfig::Get()->GetData()->UI_LoadingAssert);
	m_TAssertMap.Add(EUIType::UI_Login, SBaseConfig::Get()->GetData()->UI_LoginAssert);
	m_TAssertMap.Add(EUIType::UI_Launcher, SBaseConfig::Get()->GetData()->UI_LauncherAssert);
	m_TAssertMap.Add(EUIType::UI_CSRole, SBaseConfig::Get()->GetData()->UI_CSRoleAsset);

	FCoreUObjectDelegates::GetPreGarbageCollectDelegate().AddRaw(this, &FWidgetManager::OnPreGarbageCollect);

}

void FWidgetManager::StartGame()
{

}

void FWidgetManager::Shutdown()
{
	for (TWidgetMap::TIterator It(m_TWidgetMap); It; ++It)
	{
		UBaseWidget* pWidget = It.Value();

		//判断是否已经被GC，GC调后清除
		if (pWidget->IsRooted())
			pWidget->RemoveFromRoot();

		It.RemoveCurrent();
	}

	FCoreUObjectDelegates::GetPreGarbageCollectDelegate().RemoveAll(this);

	m_TWidgetMap.Empty();
	m_TAssertMap.Empty();

}

UBaseWidget* FWidgetManager::Show(EUIType::Type UiType, TCHAR* AssertName /*= NULL*/, int32 nZOrder /*= 0*/)
{
	FString NAssertName = AssertName == NULL ? m_TAssertMap.FindRef(UiType) : AssertName;
	if (NAssertName.IsEmpty())
		return nullptr;

	if (AssertName != NULL && !m_TAssertMap.Contains(UiType))
		m_TAssertMap.Add(UiType, NAssertName);

	UBaseWidget* pWidget = m_TWidgetMap.FindRef(UiType);
	if (!pWidget)
	{
		pWidget = UGameUtil::LoadWidget<UBaseWidget>(NAssertName, g_pGameInstance);
		if (!pWidget)
		{
			return NULL;
		}
		m_TWidgetMap.Add(UiType, pWidget);

	}

	if (pWidget->IsVisible())
		return pWidget;

	pWidget->Show(nZOrder, UiType);

	return pWidget;
}

void FWidgetManager::Hide(EUIType::Type UiType, bool bGC /*= true*/)
{
	if (UiType == EUIType::UI_All)
	{
		for (TWidgetMap::TIterator It(m_TWidgetMap); It; ++It)
		{
			//如果指针是无效的，则返回
			UBaseWidget* pWidget = It.Value();
			if (pWidget && !pWidget->IsValidLowLevel())
			{
				return;
			}

			//如果UI没有显示，则返回
			if (!pWidget || !pWidget->IsVisible())
			{
				return;
			}

			pWidget->Hide(bGC);
		}
	}
	else
	{
		UBaseWidget* pWidget = m_TWidgetMap.FindRef(UiType);

		//如果指针是无效的，则返回
		if (pWidget && !pWidget->IsValidLowLevel())
		{
			return;
		}

		//如果UI没有显示，则返回
		if (!pWidget || !pWidget->IsVisible())
		{
			return;
		}

		pWidget->Hide(bGC);
	}
}

void FWidgetManager::Refresh(EUIType::Type widget)
{
	UBaseWidget* pWidget = m_TWidgetMap.FindRef(widget);
	if (pWidget && pWidget->IsValidLowLevel())
	{
		pWidget->Refresh();
	}
}

bool FWidgetManager::IsVisible(EUIType::Type widget)
{
	UBaseWidget* pWidget = m_TWidgetMap.FindRef(widget);
	if (pWidget && pWidget->IsValidLowLevel())
	{
		return pWidget->IsVisible();
	}

	return false;
}

UBaseWidget* FWidgetManager::GetWidget(EUIType::Type UiType)
{
	UBaseWidget* pWidget = m_TWidgetMap.FindRef(UiType);
	if (pWidget && !pWidget->IsValidLowLevel())
	{
		return nullptr;
	}

	return pWidget;
}

UBaseWidget* FWidgetManager::ShowLoadingScreen()
{
	return Show(EUIType::UI_LoadingScreen, nullptr, 100);
}

void FWidgetManager::HideLoadingScreen(float fDelayTime)
{
	Hide(EUIType::UI_LoadingScreen);
}

void FWidgetManager::OnPreGarbageCollect()
{
	for (TWidgetMap::TIterator It(m_TWidgetMap); It; ++It)
	{
		//Widget被释放掉后，GC前会被清除
		UBaseWidget* pWidget = It.Value();
		if (!pWidget || (pWidget && !pWidget->IsValidLowLevel()) || (pWidget && pWidget->GetGC()))
		{
			It.RemoveCurrent();
		}
	}
}

void FWidgetManager::AddReferencedObjects(FReferenceCollector& Collector)
{
	for (TWidgetMap::TIterator It(m_TWidgetMap); It; ++It)
	{

		UBaseWidget* pWidget = It.Value();
		Collector.AddReferencedObject(pWidget);
	}
}
