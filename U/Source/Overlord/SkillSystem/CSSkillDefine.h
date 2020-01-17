// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSSkillDefine.generated.h"


/*
����Ƭ�δ�������
*/

UENUM(BlueprintType)
enum class EComboClipTriggerType :uint8
{
	OnGround = 0,
	InAir
};

/*
����״̬
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
������Ӧ����
*/
//PressImmediate ���´���
//PressImmediate ̧�𴥷�
//Time ��ʱ����
//Chanting �����κβ�����ʱ�䵽�˾ʹ���
//Charge �������£����ض�ʱ�䣬����̧�𴥷�����
//BeforeCharge ����֮ǰ
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
���ܰ�ť����
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
	//����

	float LockRange;
	float FirstLockRange;

	//����ֵ
	AActor* FindEnemy;
	FVector TargetPostion;
	FRotator TargetDirection;
};
