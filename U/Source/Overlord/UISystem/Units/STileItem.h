// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UISystem/SBaseWidget.h"
#include "SUIDefine.h"
#include "STileItem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileItemClicked, const FTileItemData&, ItemData);

UCLASS()
class OVERLORD_API UTileItem : public UBaseWidget
{
	GENERATED_BODY()

public:
	UTileItem(const FObjectInitializer& ObjectInitializer);

	virtual bool                        Init() override;
	virtual void                        Refresh() override;

private:
	UFUNCTION()
		void                                OnItemClicked(bool bIsChecked);

public:
	void								SetItemName(FString Name);
	void								SetServerStatus(int32 nStatus);
private:
	class UCheckBox* m_pItemBtn;
	class UImage* m_pServerStatusImage;
	class UTextBlock* m_pServerNameTextBox;
	class UTextBlock* m_pSerStatusNameTextBox;

public:
	UPROPERTY(BlueprintAssignable, Category = Events, meta = (DisplayName = "On Item Clicked"))
		FOnTileItemClicked					m_OnItemClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileItem", meta = (ExposeOnSpawn = true))
		int32								m_ServerStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileItem", meta = (ExposeOnSpawn = true))
		FTileItemData						m_ServerData;
	
};
