// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SUIDefine.h"
#include "SBaseWidget.generated.h"


/*******************************ÀàÉùÃ÷************************************/
// Desc 	: 
// Author	: WangPeng
// Time		: 2019.5.25
/************************************************************************/


UCLASS()
class OVERLORD_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBaseWidget(const FObjectInitializer& ObjectInitializer);

private:
	bool                                m_bInitialized;
	EUIType::Type						m_eType;
	bool								m_bCanGC;
public:

	virtual void                        NativeConstruct() override;

	virtual void						NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void                        BeginDestroy() override;

	virtual bool                        Init() { return true; }

	virtual void                        Show(int32 nZOrder = 0, EUIType::Type eType = EUIType::UI_None);

	virtual void                        Hide(bool bGC = true);

	virtual void                        Refresh() {}

	virtual UWidgetAnimation*			GetAnimation(FString InName);

	virtual void						SetGC(bool bGC)
	{
		m_bCanGC = bGC;
	}

	virtual bool						GetGC()
	{
		return m_bCanGC;
	}

	template<typename C = UObject>
	C* GetWidget(const FName& Name) const
	{
		return Cast<C>(GetWidgetFromName(Name));
	}
public:
	FORCEINLINE void					SetType(EUIType::Type eType)
	{
		m_eType = eType;
	}
	
};
