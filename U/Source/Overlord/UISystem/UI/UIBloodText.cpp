// Fill out your copyright notice in the Description page of Project Settings.


#include "UIBloodText.h"
#include "WidgetAnimation.h"
#include "TextBlock.h"


UUIBloodText::UUIBloodText(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

bool UUIBloodText::Init()
{
	BloodText = Cast<UTextBlock>(GetWidgetFromName(TEXT("BloodText")));
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	for (UWidgetAnimation* Animation : WidgetClass->Animations)
	{
		//ÕýÈ·µÄUIAnimation name
		FText Name = FText::FromName(Animation->GetFName());
		FString InName = "Flow";
		if (Name.ToString().Contains(InName + TEXT("_INST")))
		{
			BloodAnimation = Animation;
		}
	}
	if (BloodText == nullptr || BloodAnimation == nullptr)return false;
	return true;
}

void UUIBloodText::Play(float DamageHp)
{
	FText Damage = FText::FromString(FString::FromInt(DamageHp));
	BloodText->SetText(Damage);
	PlayAnimation(BloodAnimation);
}
