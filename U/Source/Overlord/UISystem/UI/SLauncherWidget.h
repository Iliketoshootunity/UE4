// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UISystem/SBaseWidget.h"
#include "SLauncherWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API ULauncherWidget : public UBaseWidget
{
	GENERATED_BODY()

private:
	UWidgetAnimation* m_pAnimation;
	class UTextBlock* m_pText;
public:
	ULauncherWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool                        Init() override;
	virtual void                        Refresh() override;

	virtual void                        Show(int32 nZOrder = 0, EUIType::Type eType = EUIType::UI_None) override;

	virtual void                        Hide(bool bGC = true) override;

	virtual void						OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	
};
