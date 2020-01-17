// Fill out your copyright notice in the Description page of Project Settings.


#include "UIHead.h"
#include "Components/ProgressBar.h"
#include "TextBlock.h"

UUIHead::UUIHead(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

bool UUIHead::Init()
{
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CharacterName")));
	return true;
}

void UUIHead::UpdateHp(float NewHp)
{
	Hp = NewHp;
	if (MaxHp <= 0)
	{
		return;
	}
	float Progress = Hp / MaxHp;
	HPBar->SetPercent(Progress);
}

void UUIHead::UpdateMaxHp(float NewMaxHp)
{
	MaxHp = NewMaxHp;
	if (MaxHp <= 0)
	{
		return;
	}
	float Progress = Hp / MaxHp;
	HPBar->SetPercent(Progress);
}

void UUIHead::SetCharacterName(FString NewName)
{
	FText Name = FText::FromString(NewName);
	NameText->SetText(Name);
}
