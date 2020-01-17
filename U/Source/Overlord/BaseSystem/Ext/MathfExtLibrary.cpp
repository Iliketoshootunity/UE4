// Fill out your copyright notice in the Description page of Project Settings.


#include "MathfExtLibrary.h"
#include "Kismet/KismetMathLibrary.h"

float UMathfExtLibrary::VectorAngle2D(FVector2D Pos1, FVector2D Pos2)
{
	FVector2D Pos1Normalized = UKismetMathLibrary::Normal2D(Pos1);
	FVector2D Pos2Normalized = UKismetMathLibrary::Normal2D(Pos2);
	float DotValue = UKismetMathLibrary::DotProduct2D(Pos1Normalized, Pos2Normalized);
	float Angle = UKismetMathLibrary::DegAcos(DotValue);
	return Angle;
}

