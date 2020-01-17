// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UISystem/SBaseWidget.h"
#include "SLoadingScreenWidget.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FOnAnimFinished);

UCLASS()
class OVERLORD_API ULoadingScreenWidget : public UBaseWidget
{
	GENERATED_BODY()

private:
	class UImage* m_pLoadingTexture;
	class UProgressBar* m_pLoadingProgressBar;
	class UTextBlock* m_pDescText;
	class UWidgetAnimation* m_pProcessAnimation;

	bool					m_LoadMapFinished;
	FOnAnimFinished			m_OnANimFinished;

public:
	ULoadingScreenWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool                        Init() override;
	virtual void                        Refresh() override;
	virtual void                        BeginDestroy() override;
	virtual void						NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void                        Show(int32 nZOrder = 0, EUIType::Type eType = EUIType::UI_None) override;

	virtual void                        Hide(bool bGC = true) override;

	void								SetProgressPercent(float fValue);
	void								DoAnimToFinish(float fValue = 2.0f);

	void								SetAnimFinishedEvent(const FOnAnimFinished& event);
	void								SetTexture(UTexture2D* pTexture);
	void								PlayAnim();

	UFUNCTION()
	void								AnimationFinished();
	
};
