// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameDefine.h"
#include "TableDataDefine.h"
#include "../Character/CSCharacter.h"

class ACSWeapon;
/**
 *
 */
class OVERLORD_API FCSCharacterRes
{
public:
	FCSCharacterRes();
	~FCSCharacterRes();

	typedef TMap<EActorUnit::Type, ACSCharacter*>		TActorMap;			//
	typedef TMap<BODY_PART::Type, int32>			TBodypartResMap;

	TWeakObjectPtr<ACSWeapon>  Weapon;														//主体的武器							
private:
	int32                                           m_nCharIndex;                   //Character唯一索引
	TBodypartResMap									m_PartMap;						//实际换装部位对应的Npcres部件的模型ID
	TActorMap										m_CharActorMap;               //实际的模型管理
	const FRoleTypeData* m_pCharacterTypeData;			//character资源
public:

	/************************************************************************
	* Desc 	: 初始化创建本体Actor
	* Param	:
	* Return	: 成功 ： true ， 失败 ： false
	* Author	: WangPeng
	* Time		: [3/5/2019 wp]
	************************************************************************/
	bool                                            Init(FString CharacterBPName, FVector Pos, FRotator Rot);
	//测试，玩家专有
	void											CreateWeapon();

	void											DestoryUnit();
	/************************************************************************
	* Desc 	: 设置部件
	* Param	: ePartType 部件枚举； nPartID：部件表ID
	* Return	:
	* Author	: WangPeng
	* Time		: [4/24/2019 wp]
	************************************************************************/
	void											SetPart(BODY_PART::Type ePartType, int32 nPartID);

	bool											SetRideHorse(int32 nPartID);

	void											SetLocation(FVector Loc);
	void											SetRotator(FRotator roc);
	void											SetScale(FVector scale);

	ACSCharacter* GetActorUnit();

private:
	bool											IsVaildPart(BODY_PART::Type ePartType);
	bool											IsVaildActor();

	void											RefreshPart();

	void											OnPreGarbageCollect();
};
