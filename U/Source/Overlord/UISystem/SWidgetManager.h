// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SBaseWidget.h"
#include "SSingleton.h"
#include "SUIDefine.h"

/*******************************类声明************************************/
// Desc 	: UI管理类
// Author	: WangPeng
// Time		: 2019.5.25
/************************************************************************/


class OVERLORD_API FWidgetManager : public SSingleton<FWidgetManager>, public FGCObject
{

private:
	typedef TMap<int32, FString>		TAssertMap;
	TAssertMap							m_TAssertMap;

	typedef TMap<int32, UBaseWidget*>	TWidgetMap;
	TWidgetMap							m_TWidgetMap;


public:

	void                                Init();
	void                                StartGame();
	void                                Shutdown();

	UBaseWidget*						Show(EUIType::Type UiType, TCHAR* AssertName = NULL, int32 nZOrder = 0);
	void                                Hide(EUIType::Type UiType, bool bGC = true);
	void                                Refresh(EUIType::Type widget);
	bool								IsVisible(EUIType::Type widget);
	UBaseWidget*						GetWidget(EUIType::Type UiType);

	UBaseWidget*						ShowLoadingScreen();
	void								HideLoadingScreen(float fDelayTime = 0.0f);

private:
	void								OnPreGarbageCollect();
	virtual void						AddReferencedObjects(FReferenceCollector& Collector) override;

	
};
