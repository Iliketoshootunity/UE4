// Fill out your copyright notice in the Description page of Project Settings.


#include "UICreateRole.h"
#include "EditableTextBox.h"
#include "Button.h"

UUICreateRole::UUICreateRole(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UUICreateRole::Init()
{
	RoleNameBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Account")));
	RoleNameBox->OnTextChanged.AddDynamic(this, &UUICreateRole::OnRoleNameTextChange);
	EnetrGameBtn = Cast<UButton>(GetWidgetFromName(TEXT("LoginBtn")));
	EnetrGameBtn->OnClicked.AddDynamic(this, &UUICreateRole::OnEnterGameBtnClicked);

	CurRoleName = RoleNameBox->GetText();
	return true;
}

void UUICreateRole::OnRoleNameTextChange(const FText& Text)
{
	CurRoleName = Text;
}

void UUICreateRole::OnEnterGameBtnClicked()
{

}
