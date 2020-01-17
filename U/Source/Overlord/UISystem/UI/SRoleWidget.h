// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UISystem/SBaseWidget.h"
#include "SRoleWidget.generated.h"

/**
 * 
 */
USTRUCT()
struct FRoleInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		class UCheckBox* pImageCheckBox;

	UPROPERTY()
		class UTextBlock* pRoleLevelText;

	UPROPERTY()
		class UTextBlock* pRoleNameText;
};

UCLASS()
class OVERLORD_API URoleWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	URoleWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool                        Init() override;

private:
	UFUNCTION()
		void								OnBackToLogin();

	UFUNCTION()
		void								OnEntryGame();

public:
	void								ShowSelectRoleWidget();
	void								ShowCreateRoleWidget();
private:
	UUserWidget* m_pCreateRoleWidget;
	UUserWidget* m_pSelectRoleWidget;
	class UButton* m_pSRBackBtn;
	class UButton* m_pSREntryGameBtn;
	class UButton* m_pCRBackBtn;
	class UButton* m_pHumanBtn;
	class UButton* m_pYaoBtn;
	class UButton* m_pJingLingBtn;
	class UButton* m_pCREntryGameBtn;

	TMap<int32, FRoleInfo>				m_RoleListWidget;
	
};
