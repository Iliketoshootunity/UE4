// Fill out your copyright notice in the Description page of Project Settings.


#include "SServerList.h"
#include "ScrollBox.h"
#include "ServerListConfig.h"
#include "BaseSystem/GameUtil.h"
#include "SListItem.h"
#include "SGameInstance.h"
#include "STileItem.h"

UServerList::UServerList(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	m_nTileItemCount = 10;
}

bool UServerList::Init()
{
	m_pListItems.Empty();

	m_pServerAreaScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ServerArea")));
	if (!m_pServerAreaScrollBox) return false;

	/*m_pRecentServerListItem = Cast<USTListItem>(GetWidgetFromName(TEXT("RecentServer")));
	if (!m_pRecentServerListItem) return false;

	m_pRecommendServerListItem = Cast<USTListItem>(GetWidgetFromName(TEXT("RecommendServer")));
	if (!m_pRecommendServerListItem) return false;*/

	m_pTileItems.Empty();
	for (int32 idx = 0; idx < m_nTileItemCount; ++idx)
	{
		FString WidgetName = FString::Printf(TEXT("TileItem_%d"), idx);
		UTileItem* m_pTileItem = Cast<UTileItem>(GetWidgetFromName(*WidgetName));
		if (!m_pTileItem) return false;

		m_pTileItems.Add(m_pTileItem);
	}

	ServerListConfig::FData ServerData = ServerListConfig::Get()->GetDataRef();
	if (ServerData.Num() > 0)
	{
		for (ServerListConfig::FData::TIterator It(ServerData); It; ++It)
		{
			//…Ë÷√Area
			FServerListData ServerListData = It->Value;
			if (ServerListData.ArrServerData.Num() > m_pTileItems.Num())
				continue;

			UListItem* pItem = UGameUtil::LoadWidget<UListItem>(ServerListData.ItemAssetFile, g_pGameInstance);
			if (!pItem)
			{
				return false;
			}
			pItem->m_ItemIndex = It->Key;
			pItem->m_ItemName = FText::FromString(ServerListData.AreaName);
			pItem->m_OnItemClicked.AddDynamic(this, &UServerList::OnClieckedListItem);
			pItem->Refresh();
			m_pServerAreaScrollBox->AddChild(pItem);

			m_pListItems.Add(It->Key, pItem);
		}
	}

	RefreshItemList(1);

	return true;
}

TArray<class UTileItem*> UServerList::GetTileItemWidget()
{
	return m_pTileItems;
}


void UServerList::RefreshItemList(int32 idx)
{
	ServerListConfig::FData ServerDataMap = ServerListConfig::Get()->GetDataRef();
	if (ServerDataMap.Num() <= 0) return;
	FServerListData* pServerListData = ServerDataMap.Find(idx);
	if (!pServerListData) return;

	UListItem* pItem = m_pListItems.FindRef(idx);
	if (!pItem) return;

	pItem->SetItemState(ECheckBoxState::Checked);

	//…Ë÷√list
	for (int32 i = 0; i < m_pTileItems.Num(); ++i)
	{
		UTileItem* pTileItem = m_pTileItems[i];
		if (!pTileItem)
			continue;

		if (!pServerListData->ArrServerData.IsValidIndex(i))
		{
			pTileItem->SetVisibility(ESlateVisibility::Hidden);
			continue;
		}
		FServerData* pServerData = &pServerListData->ArrServerData[i];
		pTileItem->m_ServerData.ItemName = pServerData->Name;
		pTileItem->m_ServerStatus = pServerData->Status;
		pTileItem->m_ServerData.ServerIP = pServerData->IP;
		pTileItem->m_ServerData.ServerPort = pServerData->Port;
		pTileItem->Refresh();
		pTileItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	m_nCurrentAreaIndex = idx;
}

void UServerList::ResetListItemState(int32 idx)
{
	for (TMap<int32, UListItem*>::TIterator It(m_pListItems); It; ++It)
	{
		if (It.Key() == idx)
		{
			continue;
		}

		UListItem* pItem = It.Value();
		pItem->SetItemState(ECheckBoxState::Unchecked);

	}
}

void UServerList::OnClieckedListItem(int32 index)
{
	ResetListItemState(index);
	RefreshItemList(index);
}
