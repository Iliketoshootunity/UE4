// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NpcSystem/Character/CSPlayer.h"
#include "Protoc/fight.pb.h"
#include "Protoc/move.pb.h"
#include "CSCharacter.h"
#include "CSMainPlayer.generated.h"

/**
 *
 */
UCLASS()
class OVERLORD_API ACSMainPlayer : public ACSPlayer
{
	GENERATED_BODY()
public:
	ACSMainPlayer();
public:
	//////////////////////////////////////////////////////////////////////////
	// 输入操作相关
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;		//绑定输入
	void MoveForward(float Val);																		//向前后移动
	void MoveRight(float Val);																			//左右移动					
	void Move();
	UFUNCTION(BlueprintCallable, Category = Input) void NormalAttackPressed();
	UFUNCTION(BlueprintCallable, Category = Input) void NormalAttackRelesed();
	UFUNCTION(BlueprintCallable, Category = Input) void Skill1Pressed();
	UFUNCTION(BlueprintCallable, Category = Input) void Skill1Relesed();
	UFUNCTION(BlueprintCallable, Category = Input) void SkillPressed(int32 SkillId);
	UFUNCTION(BlueprintCallable, Category = Input) void SkillRelesed(int32 SkillId);
	UFUNCTION(BlueprintCallable, Category = Input) void RollPressed();
	UFUNCTION(BlueprintCallable, Category = Input) void RollRelesed();

public:
	UFUNCTION(BlueprintPure, Category = Input) FORCEINLINE float GetRockerInputX() { return RockerX; };
	UFUNCTION(BlueprintPure, Category = Input) FORCEINLINE float GetRockerInputY() { return RockerY; };
protected:
	float RockerX;
	float RockerY;
	float RockerSize;
	int32 RockerDIV = 16;

	//////////////////////////////////////////////////////////////////////////
	// 状态机相关
public:
	virtual void PrepareComboClip() override;
	//////////////////////////////////////////////////////////////////////////
	// 消息
protected:
	void MoveMessage(FVector Target, float MaxWalkSpeed);
	UFUNCTION(BlueprintImplementableEvent, Category = MoveMessage) FVector CalMoveMessageTarget();

protected:
	float CacheMaxSpeed;
	FVector OldMoveMessagePosition;
	float MoveMessageTimer;
public:
	void SendMoveMessage(float DeltaTime);
	void SendComboMessage(int32 ComboId);
	void SendComboEndMessage();
	//////////////////////////////////////////////////////////////////////////
	// 其他
	FVector GetMainPlayrCameraPos();
};
