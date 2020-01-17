// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSComboJumpLine.h"
#include "CSSkillDefine.h"
#include "ComboClipTable.h"
#include "CSComboClip.generated.h"

class UCSSkill;
class UCSComboClip;
struct FComboJumpLineData;
typedef struct tagComboClipTableInfo FComboClipsTableData;



DECLARE_DELEGATE_ThreeParams(FCreateAndPlayComboClipDelegate, FComboClipsTableData*, FString, FComboJumpLineData)
DECLARE_DELEGATE(FChargeInterruptedDelegate)
/**
 *
 */
UCLASS(Blueprintable)
class OVERLORD_API UCSComboClip : public UObject
{
	GENERATED_BODY()
private:

	FCreateAndPlayComboClipDelegate CreateAndPlayComboClipDelegate;
	FChargeInterruptedDelegate ChargeInterruptedDelegate;

	FString JumpMontage;
	const FComboClipsTableData* ClipData;
	FComboJumpLineData PreLineData;

	//是否已经跳转
	bool bJump;

	//所有跳转的JumpLine
	UPROPERTY() TArray<UCSComboJumpLine*> JumpLines;
	//可以跳转的JumpLine的数组
	UPROPERTY() TArray<UCSComboJumpLine*>	CanJumpLines;
	//基于绝对优先可以跳转JumpLine，规则是当前CanJumpLineBaseAbsolute不会处理CanJumpLineBasePriority
	UPROPERTY() UCSComboJumpLine* CanJumpLineBaseAbsolute;
	//基于优先级的可以跳转JumpLine
	UPROPERTY() UCSComboJumpLine* CanJumpLineBasePriority;
	//将要释放的Jumpline缓存
	UPROPERTY() TArray<UCSComboJumpLine*> CacheReleaseJumpLines;
public:
	FORCEINLINE int32 GetId() { return ClipData->ID; }
	FORCEINLINE FString GetJumpMontage() { return JumpMontage; }
	FORCEINLINE FComboClipsTableData* GetClipData() { return (FComboClipsTableData*)ClipData; }
	FORCEINLINE FComboJumpLineData GetPreLineData() { return PreLineData; }
	//是否是跳转
	bool IsCharge();
public:
	//初始化
	void Init(const FComboClipsTableData* ClipData, FString JumpMontageName, FComboJumpLineData LineData);
	//Tick
	void Tick();
	//清除将要释放的Jumpline缓存
	void RemoveCacheReleaseJumpLines();
	//响应输入
	void RespondInput(EInputStatus InputStatus);
	//响应构建ComboJumpLine
	void RespondConstructComboJumpLine(FComboJumpLineData NewLineData);
	//响应不激活ComboJumpLine
	void RespondDeactiveComboJumpLine(int32 Index);
	//响应跳转到下个蒙太奇
	void RespondJumpOther(int32 Index);
	//匹配输入成功，在这里会剔除一些优先级低的流水线
	void MatchInputSucceed(UCSComboJumpLine* JumpLine);
	//匹配输入失败
	void MatchInputFailed(UCSComboJumpLine* JumpLine);
	//是否在JumpLines
	bool IsContainJumpLines(UCSComboJumpLine* JumpLine);
	//设置创建并播放回调
	void SetCreateAndPlayComboClipDelegate(FCreateAndPlayComboClipDelegate NewCreateAndPlayComboClipDelegate);
	//设置打断蓄力回调
	void SetChargeInterruptedDelegate(FChargeInterruptedDelegate NewChargeInterruptedDelegate);
	//自我销毁
	void Reset();
	
};
