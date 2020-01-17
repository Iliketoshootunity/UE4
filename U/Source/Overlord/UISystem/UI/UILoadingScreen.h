// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UISystem/SBaseWidget.h"
#include "UILoadingScreen.generated.h"

/**
 *
 */
DECLARE_DELEGATE(FOnAnimFinished);
UCLASS()
class OVERLORD_API UUILoadingScreen : public UBaseWidget
{
	GENERATED_BODY()
public:
	UUILoadingScreen(const FObjectInitializer& ObjectInitializer);
private:
	class UImage* LoadingTexture;
	class UTextBlock* DescText;
	class UProgressBar* LoadingProgressBar;
	class UWidgetAnimation* ProcessAnimation;


	FOnAnimFinished			OnAnimFinished;

	float					SimulateTime;
	float					SimulateTimer;
	float					SimulateStart;
	float					SimulateEnd;
	bool					bLoadMapFinished;
	bool					bExcuteSimulate;

public:
	virtual bool                        Init() override;
	virtual void						NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void                        Show(int32 nZOrder = 0, EUIType::Type eType = EUIType::UI_None) override;
	virtual void                        Hide(bool bGC = true) override;
	void								SetProgressPercent(float Value);
	void								SetAnimFinishedEvent(const FOnAnimFinished& event);
	void								OnLoadMapStart();
	void								OnLoadMapFinshed();
};
