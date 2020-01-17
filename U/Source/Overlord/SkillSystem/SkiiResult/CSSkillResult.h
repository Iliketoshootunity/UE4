// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Protoc/fight.pb.h"
#include "Engine/EngineTypes.h"
#include "../CSSkillData.h"
#include "CSSkillResult.generated.h"

/*
* ���ؿͻ������У��������ϴ�������������������֤����֤�ɹ������ķ������й������뱻�����ߵ�λ��Ϊ�����е�λ�ã��·��������ͻ���
* ����˲��Ŷ�������λ��ƫ�ƽ��н�����ͬ��Ѫ������
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
	float KeepDistance;						//�Ƿ���Ҫ����λ��,��Щ������Ҫ�����ߺͱ������߱���һ������
	float KeepDistanceTime;
	const FComboClipsTableData* ClipData;
	const FHitPerformaceTableData* HitData;

	FString HurtClipName;
public:
	FORCEINLINE FString GetHurtClipName() { return HurtClipName; }
public:
	//���ض���ҳ�ʼ��
	void LocalInit(FSkillReceiveInfo ReceiveInfo);
	//�������ҳ�ʼ��
	void ProxyInit(ACSCharacter* InAttacker, ACSCharacter* InBeAttacker, fightV2::ResFightResult* InFightResult, fightV2::HurtTarget* InHurtTarget);
};
