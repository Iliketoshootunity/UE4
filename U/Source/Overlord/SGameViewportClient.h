// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "Engine/EngineTypes.h"
#include "SGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API USGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

private:
	TMap<UObject*, FTimerHandle>			m_TDelayRemoveViewportMap;		//延时删除数据
	TMap<UObject*, FTimerHandle>			m_TDelayAddToViewportMap;		//延时显示数据
	class UBaseWidget*						m_CurrentWidget;
private:
	virtual void AddViewportWidgetContent(TSharedRef<class SWidget> ViewportContent, const int32 ZOrder = 0) override;
	virtual void RemoveViewportWidgetContent(TSharedRef<class SWidget> ViewportContent) override;

	virtual void Tick(float DeltaTime);
	virtual void BeginDestroy() override;

	void		 DelayAddToViewport(UObject* pObject, const int32 ZOrder);
	void		 DelayRemoveFromViewport(UObject* pWidget);
public:
	USGameViewportClient(const FObjectInitializer& ObjectInitializer);

	//单例，获取实例
	static USGameViewportClient* Get();

	//widget添加到Viewport，显示
	virtual void AddToViewport(class UBaseWidget* pWidget, const int32 ZOrder = 0, float fDelayTime = 0.0f);

	//从Viewport移除widget
	virtual void RemoveFromViewport(class UBaseWidget* pWidget = nullptr, float fDelayTime = 0.0f);
	
};
