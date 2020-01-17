// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UISystem/SBaseWidget.h"
#include "SServerList.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API UServerList : public UBaseWidget
{
	GENERATED_BODY()

public:
	UServerList(const FObjectInitializer& ObjectInitializer);

	virtual bool                        Init() override;

	TArray<class UTileItem*> 			GetTileItemWidget();

private:
	void								RefreshItemList(int32 idx);

	void								ResetListItemState(int32 idx);

	UFUNCTION()
		void								OnClieckedListItem(int32 index);

private:
	class UScrollBox*					m_pServerAreaScrollBox;
	class UListItem*					m_pRecentServerListItem;			//×î½üµÇÂ½
	class UListItem*					m_pRecommendServerListItem;			//ÍÆ¼öµÇÂ½
	TArray<class UTileItem*>			m_pTileItems;
	TMap<int32, class UListItem*>		m_pListItems;

	int32								m_nCurrentAreaIndex;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileItem", meta = (ExposeOnSpawn = true))
		int32								m_nTileItemCount;
	
};
