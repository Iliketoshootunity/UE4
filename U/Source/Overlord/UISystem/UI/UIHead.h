// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIHead.generated.h"

class UProgressBar;
class UTextBlock;
/**
 *
 */
UCLASS()
class OVERLORD_API UUIHead : public UUserWidget
{
	GENERATED_BODY()
public:
	UUIHead(const FObjectInitializer& ObjectInitializer);
protected:
	UProgressBar* HPBar;
	UTextBlock* NameText;
	float Hp = -1;
	float MaxHp = -1;
public:
	virtual bool  Init();
	virtual void UpdateHp(float NewHp);
	virtual void UpdateMaxHp(float NewMaxHp);
	virtual void SetCharacterName(FString NewName);
};
