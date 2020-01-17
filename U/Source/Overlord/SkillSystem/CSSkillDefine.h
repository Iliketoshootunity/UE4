// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSSkillDefine.generated.h"


/*
技能片段触发类型
*/

UENUM(BlueprintType)
enum class EComboClipTriggerType :uint8
{
	OnGround = 0,
	InAir
};

/*
输入状态
*/
UENUM(BlueprintType)
enum class EInputStatus :uint8
{
	None,
	Press = 0,
	Relese,
	Persistent,
};

/*
技能响应类型
*/
//PressImmediate 按下触发
//PressImmediate 抬起触发
//Time 计时触发
//Chanting 无需任何操作，时间到了就触发
//Charge 持续按下，到特定时间，或者抬起触发触发
//BeforeCharge 蓄力之前
UENUM(BlueprintType)
enum class EComboJumLineRespondType :uint8
{
	PressImmediate = 0,
	ReleseImmediate,
	Time,
	Chanting,
	Charge,
	BeforeCharge,
	AutoSuccessful,
};

/*
技能按钮类型
*/
UENUM(BlueprintType)
enum class ESkillButtonType :uint8
{
	None = 0,
	NormalAttackBtn,
	SkillAttackBtn,
	DodgeBtn,
};

class FSearchEnemyData
{
public:
	//参数

	float LockRange;
	float FirstLockRange;

	//返回值
	AActor* FindEnemy;
	FVector TargetPostion;
	FRotator TargetDirection;
};
