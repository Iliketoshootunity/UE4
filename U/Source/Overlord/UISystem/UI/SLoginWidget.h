// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UISystem/SBaseWidget.h"
#include "SUIDefine.h"
#include "SLoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API ULoginWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	ULoginWidget(const FObjectInitializer& ObjectInitializer);

private:
	class UButton* m_pLoginBtn;
	class UEditableTextBox* m_pAccountBox;
	class UEditableTextBox* m_pPasswordBox;
	class UButton* m_pCloseBtn;
	class UButton* m_pForgetPasswordBtn;
	class UButton* m_pTouristLoginBtn;
	class UButton* m_pRegistBtn;
	class UButton* m_pAccSecurityBtn;

	UUserWidget* m_pLogin;
	UUserWidget* m_pSelectServer;
	class UServerList* m_pServerList;
	class UButton* m_pSelectServerBtn;
	class UButton* m_pToGameBtn;
	class UTextBlock* m_pServerNameTextBox;

	FText								m_CurAccountText;
	FText								m_CurPasswordText;

	FTileItemData						m_CurServerData;

public:
	virtual bool                        Init() override;
	virtual void                        Refresh() override;

public:
	UFUNCTION()
		void                                OnLoginBtnClicked();

	UFUNCTION()
		void                                OnCloseClicked();

	UFUNCTION()
		void								OnOpenServerList();

	UFUNCTION()
		void								OnEntryCreateRole();

	UFUNCTION()
		void								OnAccBoxTextChange(const FText& Text);

	UFUNCTION()
		void								OnPasswordBoxTextChange(const FText& Text);

	UFUNCTION()
		void                                OnTileItemClicked(const FTileItemData& ItemData);
	
};
