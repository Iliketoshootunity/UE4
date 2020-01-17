// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSSkillDefine.h"
#include "ComboClipTable.h"
#include "Tickable.h"
#include "CSSkillData.h"
#include "CSComboJumpLine.generated.h"

class UCSComboClip;


UCLASS()
class OVERLORD_API UCSComboJumpLine : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

private:

	FComboJumpLineData Data;
	//表里面的其他信息，传给跳转的片段的使用的
	const FComboClipTableData* ClipData;

	bool bStartCharge;
	bool bStartTime;
	bool bMatchCharge;
	bool bCanJump;
	float ChargeTimer;
	float TimeTimer;
	UPROPERTY() UCSComboClip* OwnClip;
	EInputStatus CurInputStatus;
public:
	FORCEINLINE int32 GetID() { return Data.ID; }
	FORCEINLINE int32 GetPriority() { return Data.Priority; }
	FORCEINLINE int32 GetIndex() { return Data.Index; }
	FORCEINLINE const EComboJumLineRespondType GetRespondType() { return Data.RespondType; }

	FORCEINLINE const FComboClipTableData* GetComboClipsTableData() { return ClipData; }
	FORCEINLINE FComboJumpLineData  GetComboJumpLineData() { return Data; }
	FORCEINLINE void SetCanJump(bool CanJump) { bCanJump = CanJump; }
	FORCEINLINE bool GetCanJump() { return bCanJump; }
	FORCEINLINE UCSComboClip* GetOwnClip() { return OwnClip; }

	FString GetMontageName();
public:
	//初始化
	void Init(UCSComboClip* OwnClip, FComboJumpLineData NewData);
	//接受输入
	void RespondInput(EInputStatus InputStatus);
	//自我销毁
	void Reset();
public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
};
