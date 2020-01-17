// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UIBloodText.generated.h"


class UTextBlock;
class UWidgetAnimation;
/**
 *
 */
UCLASS()
class OVERLORD_API UUIBloodText : public UUserWidget
{
	GENERATED_BODY()
public:
	UUIBloodText(const FObjectInitializer& ObjectInitializer);
protected:
	UTextBlock* BloodText;
	UWidgetAnimation* BloodAnimation;
public:
	UFUNCTION(BlueprintCallable)
	virtual bool  Init();
	UFUNCTION(BlueprintCallable)
	void Play(float DamageHp);
};
