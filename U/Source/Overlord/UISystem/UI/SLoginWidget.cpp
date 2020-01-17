// Fill out your copyright notice in the Description page of Project Settings.


#include "SLoginWidget.h"
#include "UISystem/Units/STileItem.h"
#include "UISystem/Units/SServerList.h"
#include "TextBlock.h"
#include "Button.h"
#include "EditableTextBox.h"
#include "SGameInstance.h"
#include "Protoc/c2s.pb.h"
#include "WorldSystem/SWorldManager.h"
#include "Kismet/GameplayStatics.h"

ULoginWidget::ULoginWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool ULoginWidget::Init()
{
	m_pLogin = Cast<UUserWidget>(GetWidgetFromName(TEXT("Login")));
	if (m_pLogin == nullptr) return false;
	m_pLogin->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_pSelectServer = Cast<UUserWidget>(GetWidgetFromName(TEXT("SelectServer")));
	if (m_pSelectServer == nullptr) return false;
	m_pSelectServer->SetVisibility(ESlateVisibility::Hidden);

	m_pServerList = Cast<UServerList>(GetWidgetFromName(TEXT("ServerList")));
	if (m_pServerList == nullptr) return false;
	m_pServerList->SetVisibility(ESlateVisibility::Hidden);

	TArray<UTileItem*> TileItems = m_pServerList->GetTileItemWidget();
	for (auto pTileItem : TileItems)
	{
		pTileItem->m_OnItemClicked.AddDynamic(this, &ULoginWidget::OnTileItemClicked);
		FBasicFunction::Logs(TEXT(" ULoginWidget::Init()!"));
	}

	m_pServerNameTextBox = Cast<UTextBlock>(m_pSelectServer->GetWidgetFromName(TEXT("ServerNameTB")));
	if (!m_pServerNameTextBox) return false;

	m_pSelectServerBtn = Cast<UButton>(m_pSelectServer->GetWidgetFromName(TEXT("OpenSerListBtn")));
	if (!m_pSelectServerBtn) return false;
	m_pSelectServerBtn->OnClicked.AddDynamic(this, &ULoginWidget::OnOpenServerList);

	m_pToGameBtn = Cast<UButton>(m_pSelectServer->GetWidgetFromName(TEXT("ToGameBtn")));
	if (!m_pToGameBtn) return false;
	m_pToGameBtn->OnClicked.AddDynamic(this, &ULoginWidget::OnEntryCreateRole);

	m_pAccountBox = Cast<UEditableTextBox>(m_pLogin->GetWidgetFromName(TEXT("AccountBox")));
	if (m_pAccountBox == nullptr) return false;
	m_pAccountBox->OnTextChanged.AddDynamic(this, &ULoginWidget::OnAccBoxTextChange);

	m_pPasswordBox = Cast<UEditableTextBox>(m_pLogin->GetWidgetFromName(TEXT("Password")));
	if (m_pPasswordBox == nullptr) return false;
	m_pPasswordBox->OnTextChanged.AddDynamic(this, &ULoginWidget::OnPasswordBoxTextChange);

	m_pLoginBtn = Cast<UButton>(m_pLogin->GetWidgetFromName(TEXT("LoginBtn")));
	if (!m_pLoginBtn) return false;
	m_pLoginBtn->OnClicked.AddDynamic(this, &ULoginWidget::OnLoginBtnClicked);
	m_pLoginBtn->SetIsEnabled(false);

	m_pCloseBtn = Cast<UButton>(m_pLogin->GetWidgetFromName(TEXT("Close")));
	if (!m_pCloseBtn) return false;
	m_pCloseBtn->OnClicked.AddDynamic(this, &ULoginWidget::OnCloseClicked);

	return true;
}

void ULoginWidget::Refresh()
{

}

void ULoginWidget::OnLoginBtnClicked()
{
	m_pLogin->SetVisibility(ESlateVisibility::Hidden);
	m_pServerList->SetVisibility(ESlateVisibility::Hidden);
	m_pSelectServer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

}

void ULoginWidget::OnCloseClicked()
{
	g_pGameInstance->QuitGame();
}

void ULoginWidget::OnOpenServerList()
{
	m_pLogin->SetVisibility(ESlateVisibility::Hidden);
	m_pSelectServer->SetVisibility(ESlateVisibility::Hidden);
	m_pServerList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void ULoginWidget::OnEntryCreateRole()
{
	if (g_pGameInstance)
	{
		g_pGameInstance->DisConnectServer();
		if (!g_pGameInstance->ConnectServer(m_CurServerData.ServerIP, m_CurServerData.ServerPort))
		{
			FBasicFunction::Logs(TEXT("OnEntryCreateRole Failed!"));
			return;
		}
	}
	else
	{
		FBasicFunction::Logs(TEXT("g_pGameInstance is null!"));
		return;
	}


	LoginReq LoginReq;

	if (m_pAccountBox == nullptr)
	{
		FBasicFunction::Logs(TEXT("OnEntryCreateRole m_pAccountBox is null!"));
	}
	FString AccountName = m_pAccountBox->GetText().ToString();

	FBasicFunction::Logs(TEXT("OnEntryCreateRole AccountName is :%s"), *AccountName);


	LoginReq.set_loginname((const char*)TCHAR_TO_ANSI(*AccountName), AccountName.Len()); // ???

	LoginReq.set_sid(10);
	LoginReq.set_pid(1);
	LoginReq.set_client(1);
	LoginReq.set_ip(TCHAR_TO_ANSI(*m_CurServerData.ServerIP), m_CurServerData.ServerIP.Len());
	LoginReq.set_version(1001);
	LoginReq.set_channelid("SS");
	LoginReq.set_qudao("SSS");
	LoginReq.set_isadult(1);
	LoginReq.set_token("ASSSS");
	// 
	g_pGameInstance->SendMessage(1001, &LoginReq);

	FBasicFunction::Logs(TEXT("OnEntryCreateRole SendMessage end!"));

	/*CLoginMsg LoginCmd;
	LoginCmd.usLenght = sizeof(CLoginMsg);
	LoginCmd.usProtocol = c2s_login;

	FString AccountName = m_pAccountBox->GetText().ToString();

	FPlatformMemory::Memcpy(LoginCmd.strAccount, TCHAR_TO_ANSI(*AccountName), sizeof(AccountName));

	g_pGameInstance->GetNet()->TcpSendDataToServer(&LoginCmd, false);*/
}

void ULoginWidget::OnAccBoxTextChange(const FText& Text)
{
	if (!m_pAccountBox || !m_pLoginBtn)
		return;

	if (Text.ToString().Len() == 11)
	{
		m_pLoginBtn->SetIsEnabled(true);
	}
	else
	{
		m_pLoginBtn->SetIsEnabled(false);
	}

	if (Text.IsNumeric() || Text.IsEmpty())
	{
		if (Text.ToString().Len() > 11)
		{
			m_pAccountBox->SetText(m_CurAccountText);
		}
		else
		{
			m_CurAccountText = Text;
		}
	}
	else
	{
		m_pAccountBox->SetText(m_CurAccountText);
	}

}

void ULoginWidget::OnPasswordBoxTextChange(const FText& Text)
{
	m_CurPasswordText = Text;
}

void ULoginWidget::OnTileItemClicked(const FTileItemData& ItemData)
{
	if (m_pServerNameTextBox)
	{
		m_pServerNameTextBox->SetText(FText::FromString(ItemData.ItemName));
	}

	m_CurServerData = ItemData;

	FBasicFunction::Logs(TEXT("OnTileItemClicked!"));

	m_pLogin->SetVisibility(ESlateVisibility::Hidden);
	m_pServerList->SetVisibility(ESlateVisibility::Hidden);
	m_pSelectServer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}