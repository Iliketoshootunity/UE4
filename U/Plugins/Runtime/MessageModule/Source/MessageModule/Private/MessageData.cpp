// Fill out your copyright notice in the Description page of Project Settings.
#include "MessageData.h"

UMessagedata::UMessagedata()
{

}

void UMessagedata::Empty()
{
	Int32Datas.Empty();
	Int64Datas.Empty();
	FloatDatas.Empty();
	FStringDatas.Empty();
	ObjDatas.Empty();
	VoidRefDatas.Empty();
}
