// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UISystem/SBaseWidget.h"
#include "SServerArea.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API UServerArea : public UBaseWidget
{
	GENERATED_BODY()

public:
	UServerArea(const FObjectInitializer& ObjectInitializer);

	virtual bool                        Init() override;
	
};
