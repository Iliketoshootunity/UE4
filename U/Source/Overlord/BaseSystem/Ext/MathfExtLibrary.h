// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathfExtLibrary.generated.h"

/**
 *
 */
UCLASS()
class OVERLORD_API UMathfExtLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "MathExt|Vector2D")
	static float VectorAngle2D(FVector2D Pos1, FVector2D Pos2);
};
