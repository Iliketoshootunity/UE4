// Fill out your copyright notice in the Description page of Project Settings.


#include "SRoleWidget.h"
#include "OtherSystem/Pregame/SPregameManager.h"
#include "Button.h"
#include "CheckBox.h"
#include "TextBlock.h"

URoleWidget::URoleWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool URoleWidget::Init()
{
	m_pCreateRoleWidget = Cast<UUserWidget>(GetWidgetFromName(FName("CreateRole")));
	if (!m_pCreateRoleWidget) return false;
	m_pCreateRoleWidget->SetVisibility(ESlateVisibility::Hidden);

	m_pSelectRoleWidget = Cast<UUserWidget>(GetWidgetFromName(FName("SelectRole")));
	if (!m_pSelectRoleWidget) return false;
	m_pSelectRoleWidget->SetVisibility(ESlateVisibility::Hidden);

	m_pSRBackBtn = Cast<UButton>(m_pSelectRoleWidget->GetWidgetFromName(FName("BackLoginBtn")));
	if (!m_pSRBackBtn) return false;
	m_pSRBackBtn->OnClicked.AddDynamic(this, &URoleWidget::OnBackToLogin);

	m_pSREntryGameBtn = Cast<UButton>(m_pSelectRoleWidget->GetWidgetFromName(FName("EntryGame")));
	if (!m_pSREntryGameBtn) return false;
	m_pSREntryGameBtn->OnClicked.AddDynamic(this, &URoleWidget::OnEntryGame);

	for (int32 idx = 1; idx <= 4; ++idx)
	{
		FName NewName = *FString::Printf(TEXT("RoleInfo_%d"), idx);
		UUserWidget* pRoleInfoWidget = Cast<UUserWidget>(m_pSelectRoleWidget->GetWidgetFromName(NewName));
		if (!pRoleInfoWidget) return false;
		FRoleInfo Info;
		Info.pImageCheckBox = Cast<UCheckBox>(pRoleInfoWidget->GetWidgetFromName(TEXT("RoleCBox")));
		Info.pRoleLevelText = Cast<UTextBlock>(pRoleInfoWidget->GetWidgetFromName(TEXT("RoleLevel")));
		Info.pRoleNameText = Cast<UTextBlock>(pRoleInfoWidget->GetWidgetFromName(TEXT("RoleName")));

		m_RoleListWidget.Add(idx, Info);
	}


	m_pCRBackBtn = Cast<UButton>(m_pCreateRoleWidget->GetWidgetFromName(TEXT("GoBack")));
	if (!m_pCRBackBtn) return false;
	m_pCRBackBtn->OnClicked.AddDynamic(this, &URoleWidget::OnBackToLogin);

	m_pHumanBtn = Cast<UButton>(m_pCreateRoleWidget->GetWidgetFromName(TEXT("HumanBtn")));
	if (!m_pHumanBtn) return false;

	m_pYaoBtn = Cast<UButton>(m_pCreateRoleWidget->GetWidgetFromName(TEXT("YaoBtn")));
	if (!m_pYaoBtn) return false;

	m_pJingLingBtn = Cast<UButton>(m_pCreateRoleWidget->GetWidgetFromName(TEXT("JingLingBtn")));
	if (!m_pJingLingBtn) return false;

	m_pCREntryGameBtn = Cast<UButton>(m_pCreateRoleWidget->GetWidgetFromName(TEXT("CreateRoleToGame")));
	if (!m_pCREntryGameBtn) return false;
	m_pCREntryGameBtn->OnClicked.AddDynamic(this, &URoleWidget::OnEntryGame);

	return true;
}

void URoleWidget::OnBackToLogin()
{
	m_pCreateRoleWidget->SetVisibility(ESlateVisibility::Hidden);
	m_pSelectRoleWidget->SetVisibility(ESlateVisibility::Hidden);
	UPregameManager::Get()->ShowLogin();
}

void URoleWidget::OnEntryGame()
{
	//通知服务器进游戏
}

void URoleWidget::ShowSelectRoleWidget()
{
	m_pCreateRoleWidget->SetVisibility(ESlateVisibility::Hidden);
	m_pSelectRoleWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void URoleWidget::ShowCreateRoleWidget()
{
	m_pCreateRoleWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_pSelectRoleWidget->SetVisibility(ESlateVisibility::Hidden);
}