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
	//初始化
	void Init(UCSSkillComponent* SkillManager, const FSkillTableData* SkillData);
	//Tick
	void Tick();
	//播放
	void Play();
	//自我摧毁
	void Reset();
	//创建并播放
	void OnCreateAndPlayComboClip(FComboClipsTableData* ClipData, FString MontageName, FComboJumpLineData LineData);
	//手动打断蓄力
	void OnChargeInterrupted();
	//获取当前片段
	UCSComboClip* GetCurComboClip();
	//发送combo消息
	void SendComboMessage();
	//发送comboEnd消息
	void SendComboEndMessage();
	//接受combo消息， 同步Combo技能
	bool RespondComboMessage(int32 ComboId);

	void Destroy();
};
