// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillTable.h"
#include "SoftObjectPath.h"
#include "CSSkill.generated.h"

class UCSComboClip;
class UAnimInstance;
class UAnimMontage;
class UCSSkillComponent;
class FMapList;
class ACSCharacter;
typedef struct tagComboClipTableInfo FComboClipsTableData;



/**
 *
 */
UCLASS()
class OVERLORD_API UCSSkill : public UObject
{
	GENERATED_BODY()

private:

	int32 Id;
	const FSkillTableData* SkillData;

	UPROPERTY() UCSSkillComponent* SkillManager;
	UPROPERTY() ACSCharacter* Owner;

	FMapList* ComboList;

public:
	FORCEINLINE const FSkillTableData* GetSkillData() { return SkillData; }
	FORCEINLINE int32 GetID() {return SkillData->SkillID; }
public:
	//��ʼ��
	void Init(UCSSkillComponent* SkillManager, const FSkillTableData* SkillData);
	//Tick
	void Tick();
	//����
	void Play();
	//���Ҵݻ�
	void Reset();
	//����������
	void OnCreateAndPlayComboClip(FComboClipsTableData* ClipData, FString MontageName, FComboJumpLineData LineData);
	//�ֶ��������
	void OnChargeInterrupted();
	//��ȡ��ǰƬ��
	UCSComboClip* GetCurComboClip();
	//����combo��Ϣ
	void SendComboMessage();
	//����comboEnd��Ϣ
	void SendComboEndMessage();
	//����combo��Ϣ�� ͬ��Combo����
	bool RespondComboMessage(int32 ComboId);

	void Destroy();
};
