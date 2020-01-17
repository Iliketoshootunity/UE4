// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Messagedata.generated.h"


UCLASS()
class MESSAGEMODULE_API UMessagedata : public UObject
{
	GENERATED_BODY()
public:
	UMessagedata();
public:
	TArray<int32>	Int32Datas;
	TArray<int64>	Int64Datas;
	TArray<float>	FloatDatas;
	TArray<FString>	FStringDatas;
	UPROPERTY()
	TArray<UObject*>	ObjDatas;
	//如果有参数，强制立即发送
	TArray<void*>	VoidRefDatas;
public:
	void Empty();
};