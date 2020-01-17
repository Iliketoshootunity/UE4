// Fill out your copyright notice in the Description page of Project Settings.


#include "SListItem.h"
#include "CheckBox.h"
#include "TextBlock.h"

UListItem::UListItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UListItem::Init()
{
	m_pItemBtn = Cast<UCheckBox>(GetWidgetFromName(TEXT("ItemBtn")));
	if (!m_pItemBtn) return false;
	m_pItemBtn->SetCheckedState(ECheckBoxState::Unchecked);
	m_pItemBtn->WidgetStyle.SetCheckBoxType(ESlateCheckBoxType::ToggleButton);
	m_pItemBtn->OnCheckStateChanged.AddDynamic(this, &UListItem::OnItemClicked);

	m_pItemNameTextBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("ListItemName")));
	if (!m_pItemNameTextBox) return false;
	m_pItemNameTextBox->SetText(m_ItemName);

	return true;
}

void UListItem::Refresh()
{
	if (m_pItemNameTextBox)
	{
		m_pItemNameTextBox->SetText(m_ItemName);
	}
}

void UListItem::SetItemState(ECheckBoxState state)
{
	if (m_pItemBtn)
		m_pItemBtn->SetCheckedState(state);
}

void UListItem::SetAreaName(FString Name)
{
	if (m_pItemNameTextBox)
	{
		m_ItemName = FText::FromString(Name);
		m_pItemNameTextBox->SetText(m_ItemName);
	}

}

void UListItem::OnItemClicked(bool bIsChecked)
{
	if (m_OnItemClicked.IsBound() && bIsChecked)
	{
		m_OnItemClicked.Broadcast(m_ItemIndex);
	}

	if (m_pItemBtn && !bIsChecked)
		m_pItemBtn->SetCheckedState(ECheckBoxState::Checked);
}

void UListItem::ResetItemState()
{
	if (!m_pItemBtn) return;

	m_pItemBtn->SetCheckedState(ECheckBoxState::Unchecked);
}
