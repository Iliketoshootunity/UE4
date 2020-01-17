// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UISystem/SBaseWidget.h"
#include "UILogin.generated.h"

class FText;

/**
 *
 */
UCLASS()
class OVERLORD_API UUILogin : public UBaseWidget
{
	GENERATED_BODY()
public:
	UUILogin(const FObjectInitializer& ObjectInitializer);
private:
	class UEditableTextBox* IpBox;
	class UEditableTextBox* ProtBox;
	class UEditableTextBox* AcountBox;
	class UButton* LoginBtn;
	 FText CurIp;
	 FText CurProt;
	 FText CurAcccount;


public:
	virtual bool                        Init() override;
public:
	UFUNCTION() void OnIpBoxTextChange(const  FText& Text);
	UFUNCTION() void OnProtBoxTextChange(const  FText& Text);
	UFUNCTION() void OnAccBoxTextChange(const  FText& Text);
	UFUNCTION() void OnLoginBtnClicked();
};