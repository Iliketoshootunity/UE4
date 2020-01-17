// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SGameInstance.h"
#include "SGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API UGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UGameSingleton();

	static UGameSingleton*		Get();

	void						Init();

	void						StartGame();

	virtual	void				BeginDestroy() override;

	void						Tick(float fDeltaTime);
	
};
