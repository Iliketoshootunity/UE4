// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NpcsTable.h"
#include "TableDataDefine.h"
#include "MessageData.h"
#include "MessageDispatcher.h"
#include "../Other/CSCharacterDefine.h"
#include "CSCharacterRes.h"
#include "CSGameCharacter.generated.h"

/**
 *
 */


UCLASS()
class OVERLORD_API UCSGameCharacter : public UObject
{
	GENERATED_BODY()
public:
	UCSGameCharacter();

protected:

	FCSCharacterRes	DataResMgr;                          // 客户端资源管理器（模型、装备、图像、声音）
	//////////////////////////////////////////////////////////////////////////
	//  basic attribute
protected:
	int64 ID;
	ECharaterType CharacterType;
	float HP = 0;
	float MaxHp = 0;
	FString CharacterName;

public:
	virtual FORCEINLINE int64 GetID() { return ID; };
	virtual FORCEINLINE ECharaterType GetCharacterType() { return CharacterType; };
	virtual FORCEINLINE float GetHp() { return HP; };
	virtual	FORCEINLINE void SetHP(float NewHP) {
		if (NewHP == HP)return;
		float OldHP = HP;
		HP = NewHP;
		//发送消息
		UMessagedata* MessageData = NewObject<UMessagedata>();
		MessageData->Int64Datas.Add(ID);
		MessageData->Int64Datas.Add(OldHP);
		MessageData->Int64Datas.Add(HP);
		FMessageDispatcher::SendMessage(FMessageDefine::CharacterHPChange, MessageData);
	};
	virtual FORCEINLINE float GetMaxHp() { return MaxHp; };
	virtual FORCEINLINE void SetMaxHP(float NexMaxHp)
	{
		if (MaxHp == NexMaxHp)return;
		float OldMaxHp = MaxHp;
		MaxHp = NexMaxHp;
		//发送消息
		UMessagedata* MessageData = NewObject<UMessagedata>();
		MessageData->Int64Datas.Add(ID);
		MessageData->Int64Datas.Add(OldMaxHp);
		MessageData->Int64Datas.Add(MaxHp);
		FMessageDispatcher::SendMessage(FMessageDefine::CharacterMaxHPChange, MessageData);
	}
	virtual FORCEINLINE FString GetCharacterName() { return CharacterName; }
	virtual FORCEINLINE void SetCharacterName(FString NewCharacterName)
	{
		CharacterName = NewCharacterName;
		//发送消息
		UMessagedata* MessageData = NewObject<UMessagedata>();
		MessageData->Int64Datas.Add(ID);
	}
	//////////////////////////////////////////////////////////////////////////
	//  Table Data
public:
	const FNpcsTableData* CharacterTableData;

	virtual	FORCEINLINE const FNpcsTableData*  GetCharacterTableData()
	{
		return CharacterTableData;
	}

	const FRoleTypeData* CharacterTypeData;

	virtual FORCEINLINE const FRoleTypeData* GetCharacterTypeData()
	{
		return CharacterTypeData;
	}
	//////////////////////////////////////////////////////////////////////////
	//  Net Data


	//////////////////////////////////////////////////////////////////////////
	//  Cache Data
protected:
	FVector InitPos;
	FRotator InitRot;
public:
	virtual FORCEINLINE FVector GetInitPos()
	{
		return InitPos;
	}

	
	virtual FORCEINLINE FRotator GetInitRot()
	{
		return InitRot;
	}
public:


	FORCEINLINE	FCSCharacterRes* GetResManager()
	{
		return &DataResMgr;
	}
public:

	virtual void Init(void* Info);
};
