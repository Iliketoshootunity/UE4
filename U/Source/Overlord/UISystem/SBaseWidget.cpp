// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseWidget.h"
#include "BasicFunction.h"

UBaseWidget::UBaseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, m_bInitialized(false)
	, m_eType(EUIType::UI_None)
	, m_bCanGC(false)
{
}

void UBaseWidget::NativeConstruct()
{
	if (m_bInitialized)
	{
		return;
	}

	if (!Init())
	{
		FBasicFunction::Logs(TEXT("%s Widget Init Failed !!!"), *(GetClass()->GetFullName()));
		return;
	}

	m_bInitialized = true;

	Super::NativeConstruct();
}

void UBaseWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBaseWidget::BeginDestroy()
{
	m_bInitialized = false;
	m_eType = EUIType::UI_None;
	m_bCanGC = false;
	Super::BeginDestroy();
}

void UBaseWidget::Show(int32 nZOrder /*= 0*/, EUIType::Type eType /*= EUIType::UI_None*/)
{
	m_eType = eType;

	if (!IsVisible())
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	AddToViewport(nZOrder);

	//AddToRoot();
}

void UBaseWidget::Hide(bool bGC /*= true*/)
{
	if (IsVisible())
	{
		SetVisibility(ESlateVisibility::Hidden);
	}

	RemoveFromViewport();

	if (IsRooted())
	{
		RemoveFromRoot();
	}

	SetGC(bGC);
}

UWidgetAnimation* UBaseWidget::GetAnimation(FString InName)
{
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	for (UWidgetAnimation* Animation : WidgetClass->Animations)
	{
		//ÕýÈ·µÄUIAnimation name
		FText Name = FText::FromName(Animation->GetFName());
		if (Name.ToString().Contains(InName + TEXT("_INST")))
		{
			return Animation;
		}
	}

	return nullptr;
}
