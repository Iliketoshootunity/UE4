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

	TWeakObjectPtr<ACSWeapon>  Weapon;														//���������							
private:
	int32                                           m_nCharIndex;                   //CharacterΨһ����
	TBodypartResMap									m_PartMap;						//ʵ�ʻ�װ��λ��Ӧ��Npcres������ģ��ID
	TActorMap										m_CharActorMap;               //ʵ�ʵ�ģ�͹���
	const FRoleTypeData* m_pCharacterTypeData;			//character��Դ
public:

	/************************************************************************
	* Desc 	: ��ʼ����������Actor
	* Param	:
	* Return	: �ɹ� �� true �� ʧ�� �� false
	* Author	: WangPeng
	* Time		: [3/5/2019 wp]
	************************************************************************/
	bool                                            Init(FString CharacterBPName, FVector Pos, FRotator Rot);
	//���ԣ����ר��
	void											CreateWeapon();

	void											DestoryUnit();
	/************************************************************************
	* Desc 	: ���ò���
	* Param	: ePartType ����ö�٣� nPartID��������ID
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
