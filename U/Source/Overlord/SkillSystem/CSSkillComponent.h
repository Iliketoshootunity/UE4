// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSSkillDefine.h"
#include "CSSkillData.h"
#include "CSSkillComponent.generated.h"

class UCSSkill;
class ACSCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class OVERLORD_API UCSSkillComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	//��ӵ�е����еļ���
	UPROPERTY() TArray<UCSSkill*> SkillArray;
	//��ǰʹ�õļ���
	UPROPERTY() UCSSkill* CurSkill;
	//������ɫ
	UPROPERTY() ACSCharacter* OwnerCharacter;

	bool bSkillInput;
	int32 InputSkillId;
	ESkillButtonType SkillButtonType;
public:
	// Sets default values for this component's properties
	UCSSkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

	virtual void SkillTick();
public:
	FORCEINLINE UCSSkill* GetCurSkill() { return CurSkill; }

public:

	
	virtual void Init();

	UFUNCTION(BlueprintCallable)  void RespondInput(ESkillButtonType ButtonType, EInputStatus InputStatus, int32 SkillID);

	UFUNCTION(BlueprintCallable)  void RespondConstructComboJumpLine(FComboJumpLineData Data);

	UFUNCTION(BlueprintCallable)  void RespondDeactiveComboJumpLine(int32 Index);

	UFUNCTION(BlueprintCallable)  void RespondJumpOther(int32 Index);

	/*����Combo�ͷŵ���Ϣ*/
	void RespondComboMessage(int32 ComboId);

	void CurSkillFinished();

	void Destroy();




};
