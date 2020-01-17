// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Protoc/fight.pb.h"
#include "Engine/EngineTypes.h"
#include "../CSSkillData.h"
#include "CSSkillResult.generated.h"

/*
* 本地客户端先行，计算结果上传服务器，服务器做验证，验证成功，更改服务器中攻击者与被攻击者的位置为攻击中的位置，下发到其他客户端
* 代理端播放动作，对位置偏移进行矫正，同步血量数据
*/
class ACSCharacter;
class UCSSkill;
class UCSComboClip;

typedef struct tagComboClipTableInfo FComboClipsTableData;
typedef struct tagHitPerformaceTableInfo FHitPerformaceTableData;
/**
 *
 */
UCLASS()
class OVERLORD_API UCSSkillResult : public UObject
{
	GENERATED_BODY()
public:
	UCSSkillResult();
protected:

	FSkillReceiveInfo ReceiveInfo;
	TWeakObjectPtr<ACSCharacter> Attacker;
	TWeakObjectPtr<ACSCharacter> BeAttacker;
	TWeakObjectPtr<UCSSkill> Skill;
	TWeakObjectPtr<UCSComboClip> ComboClip;
	float KeepDistance;						//是否需要调整位置,有些技能需要攻击者和被攻击者保持一定距离
	float KeepDistanceTime;
	const FComboClipsTableData* ClipData;
	const FHitPerformaceTableData* HitData;

	FString HurtClipName;
public:
	FORCEINLINE FString GetHurtClipName() { return HurtClipName; }
public:
	//本地端玩家初始化
	void LocalInit(FSkillReceiveInfo ReceiveInfo);
	//代理端玩家初始化
	void ProxyInit(ACSCharacter* InAttacker, ACSCharacter* InBeAttacker, fightV2::ResFightResult* InFightResult, fightV2::HurtTarget* InHurtTarget);
};
