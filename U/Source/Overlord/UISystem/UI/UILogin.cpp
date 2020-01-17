// Fill out your copyright notice in the Description page of Project Settings.


#include "UILogin.h"
#include "EditableTextBox.h"
#include "Button.h"
#include "BasicFunction.h"
#include "SGameInstance.h"
#include "Protoc/c2s.pb.h"
#include "StringConv.h"
#include "BaseSystem/GameUtil.h"
#include "WorldSystem/SWorldManager.h"

UUILogin::UUILogin(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}
bool UUILogin::Init()
{
	IpBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("IP")));
	IpBox->OnTextChanged.AddDynamic(this, &UUILogin::OnIpBoxTextChange);
	ProtBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Port")));
	ProtBox->OnTextChanged.AddDynamic(this, &UUILogin::OnProtBoxTextChange);
	AcountBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Account")));
	AcountBox->OnTextChanged.AddDynamic(this, &UUILogin::OnAccBoxTextChange);
	LoginBtn = Cast<UButton>(GetWidgetFromName(TEXT("LoginBtn")));
	LoginBtn->OnClicked.AddDynamic(this, &UUILogin::OnLoginBtnClicked);

	CurIp = IpBox->GetText();	
	CurProt = ProtBox->GetText();
	CurAcccount = AcountBox->GetText();

	return true;
}

void UUILogin::OnIpBoxTextChange(const FText& Text)
{
	CurIp = Text;
}

void UUILogin::OnProtBoxTextChange(const FText& Text)
{
	CurProt = Text;
}

void UUILogin::OnAccBoxTextChange(const FText& Text)
{
	CurAcccount = Text;
}

void UUILogin::OnLoginBtnClicked()
{
	if (g_pGameInstance)
	{
		g_pGameInstance->DisConnectServer();
		FString CurProtStr = CurProt.ToString();
		if (!g_pGameInstance->ConnectServer(CurIp.ToString(), FCString::Atoi(*CurProtStr)))
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

	if (IpBox == nullptr || ProtBox == nullptr || AcountBox == nullptr)
	{
		FBasicFunction::Logs(TEXT("OnEntryCreateRole m_pAccountBox is null!"));
	}
	FString AccountName = CurAcccount.ToString();

	FBasicFunction::Logs(TEXT("OnEntryCreateRole AccountName is :%s"), *AccountName);


	LoginReq.set_loginname((const char*)TCHAR_TO_ANSI(*AccountName), AccountName.Len()); // ???

	LoginReq.set_sid(10);
	LoginReq.set_pid(1);
	LoginReq.set_client(1);
	LoginReq.set_ip(TCHAR_TO_ANSI(*CurIp.ToString()), CurIp.ToString().Len());
	LoginReq.set_version(1001);
	LoginReq.set_channelid("SS");
	LoginReq.set_qudao("SSS");
	LoginReq.set_isadult(1);
	LoginReq.set_token("ASSSS");
 
	g_pGameInstance->SendMessage(1001, &LoginReq);

	

	FBasicFunction::Logs(TEXT("OnEntryCreateRole SendMessage end!"));
}
