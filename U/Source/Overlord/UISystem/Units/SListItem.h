// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UISystem/SBaseWidget.h"
#include "SListItem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemClicked, int32, Index);

UCLASS()
class OVERLORD_API UListItem : public UBaseWidget
{
	GENERATED_BODY()

public:
	UListItem(const FObjectInitializer& ObjectInitializer);

private:
	class UCheckBox* m_pItemBtn;
	class UTextBlock* m_pItemNameTextBox;
public:
	UPROPERTY(BlueprintAssignable, Category = Events, meta = (DisplayName = "On Item Clicked"))
		FOnItemClicked						m_OnItemClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ListItem", meta = (ExposeOnSpawn = true))
		FText								m_ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ListItem", meta = (ExposeOnSpawn = true))
		int32								m_ItemIndex;

public:
	virtual bool                        Init() override;
	virtual void                        Refresh() override;

	void								SetItemState(ECheckBoxState state);
	void								SetAreaName(FString Name);

private:
	UFUNCTION()
		void                                OnItemClicked(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = "ListItem")
		void								ResetItemState();
	
};
