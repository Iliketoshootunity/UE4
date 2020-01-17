// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSSkillDefine.h"
#include "Engine/EngineTypes.h"
#include "CSSkillData.generated.h"

class UCSComboClip;
class ACSCharacter;
class UCSSkill;

USTRUCT(BlueprintType)
struct FComboJumpLineData
{
	GENERATED_BODY()

public:
	//跳转逻辑相关数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JumpInfo)    int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JumpInfo)	int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JumpInfo)	int32 Priority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JumpInfo)	FString JumpMontageName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JumpInfo)	EComboJumLineRespondType RespondType;
	//跳转的Clip的数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClipInfo)	float LockRange = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClipInfo)	float FirsetLock = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClipInfo)	float HasRockerAngleLimit = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClipInfo)	float NoRockerAngleLimit = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClipInfo)	float MaxRotateAngle = 180;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClipInfo)	TEnumAsByte<enum EMovementMode> MovementMode = EMovementMode::MOVE_Walking;
};

UENUM(BlueprintType)
enum class EAtkShapeType :uint8
{
	Box = 0,
	Sphere,
	Sector
};

USTRUCT(BlueprintType)
struct FHitCheckInfo
{
	GENERATED_BODY()
public:
	EAtkShapeType ShapeType;
	FVector LocaPosOffset;
	float ScaleX;
	float ScaleY;
	float ScaleZ;
	int32 HitPerformaceId;
	FRotator HitFXRotate;
};

USTRUCT(BlueprintType)
struct FSkillReceiveInfo
{
	GENERATED_BODY()
public:
	TWeakObjectPtr<ACSCharacter> Attacker;
	TWeakObjectPtr<AActor> BeAttacker;
	TWeakObjectPtr<UCSSkill> Skill;
	TWeakObjectPtr<UCSComboClip> ComboClip;
	FHitResult HitResult;
	FHitCheckInfo HitCheckInfo;
};
