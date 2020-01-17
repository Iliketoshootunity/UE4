// Fill out your copyright notice in the Description page of Project Settings.


#include "STileItem.h"
#include "CheckBox.h"
#include "TextBlock.h"
#include "Image.h"
#include "BaseConfig.h"
#include "BaseSystem/GameUtil.h"
#include "Engine/Texture2D.h"

UTileItem::UTileItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UTileItem::Init()
{
	m_pItemBtn = Cast<UCheckBox>(GetWidgetFromName(TEXT("ItemBtn")));
	if (!m_pItemBtn) return false;
	m_pItemBtn->SetCheckedState(ECheckBoxState::Unchecked);
	m_pItemBtn->WidgetStyle.SetCheckBoxType(ESlateCheckBoxType::ToggleButton);
	m_pItemBtn->OnCheckStateChanged.AddDynamic(this, &UTileItem::OnItemClicked);

	m_pServerStatusImage = Cast<UImage>(GetWidgetFromName(TEXT("ServerStatusTex")));
	if (!m_pServerStatusImage) return false;

	m_pServerNameTextBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("ServerName")));
	if (!m_pServerNameTextBox) return false;
	m_pServerNameTextBox->SetText(FText::FromString(m_ServerData.ItemName));

	m_pSerStatusNameTextBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("ServerStatusName")));
	if (!m_pSerStatusNameTextBox) return false;

	const FServerStatusData* pServerStatusData = SBaseConfig::Get()->GetServerStatusData(m_ServerStatus);
	if (pServerStatusData)
	{
		UTexture2D* pTex = UGameUtil::LoadClass<UTexture2D>(pServerStatusData->ServerStatusAssetFile);
		m_pServerStatusImage->SetBrushFromTexture(pTex);

		m_pSerStatusNameTextBox->SetText(FText::FromString(pServerStatusData->ServerStatusName));
	}

	return true;
}

void UTileItem::Refresh()
{
	SetItemName(m_ServerData.ItemName);

	SetServerStatus(m_ServerStatus);
}

void UTileItem::OnItemClicked(bool bIsChecked)
{
	if (m_OnItemClicked.IsBound())
	{
		m_OnItemClicked.Broadcast(m_ServerData);
	}
}

void UTileItem::SetItemName(FString Name)
{
	if (m_pServerNameTextBox)
	{
		m_pServerNameTextBox->SetText(FText::FromString(m_ServerData.ItemName));
	}
}

void UTileItem::SetServerStatus(int32 nStatus)
{
	if (!m_pSerStatusNameTextBox) return;

	m_ServerStatus = nStatus;
	const FServerStatusData* pServerStatusData = SBaseConfig::Get()->GetServerStatusData(m_ServerStatus);
	if (pServerStatusData)
	{
		UTexture2D* pTex = UGameUtil::LoadClass<UTexture2D>(pServerStatusData->ServerStatusAssetFile);
		m_pServerStatusImage->SetBrushFromTexture(pTex);

		m_pSerStatusNameTextBox->SetText(FText::FromString(pServerStatusData->ServerStatusName));
	}
}