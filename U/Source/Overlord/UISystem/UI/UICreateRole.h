// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UISystem/SBaseWidget.h"
#include "UICreateRole.generated.h"


class FText;
/**
 *
 */
UCLASS()
class OVERLORD_API UUICreateRole : public UBaseWidget
{
	GENERATED_BODY()
public:
	UUICreateRole(const FObjectInitializer& ObjectInitializer);
private:
	class UEditableTextBox* RoleNameBox;
	class UButton* EnetrGameBtn;
private:
	 FText CurRoleName;
public:
	virtual bool                        Init() override;
public:
	UFUNCTION() void OnRoleNameTextChange(const  FText& Text);
	UFUNCTION() void OnEnterGameBtnClicked();
};
