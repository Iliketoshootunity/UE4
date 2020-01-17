// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSingleton.h"
#include "TableDataDefine.h"

/**
 * 
 */
typedef struct tagNpcResData
{
public:
	typedef TMap<int32, class FRolePartMeshTable*>			TBodyPartMeshMap;
	typedef TMap<int32, class FExtraRolePartMeshTable*>		TExtraBodyPartMeshMap;

	TExtraBodyPartMeshMap			m_ExtraBodyPartMeshMap;
	TBodyPartMeshMap				m_BodyPartMeshMap;

	tagNpcResData()
	{
		m_ExtraBodyPartMeshMap.Empty();
		m_BodyPartMeshMap.Empty();
	}

	~tagNpcResData()
	{
		for (TExtraBodyPartMeshMap::TIterator It(m_ExtraBodyPartMeshMap); It; ++It)
		{
			delete It.Value();
			It.RemoveCurrent();
		}

		m_ExtraBodyPartMeshMap.Empty();

		for (TBodyPartMeshMap::TIterator It(m_BodyPartMeshMap); It; ++It)
		{
			delete It.Value();
			It.RemoveCurrent();
		}

		m_ExtraBodyPartMeshMap.Empty();
	}
}FNpcResData;

class TABLEMODULE_API FRolePartMeshManager : public SSingleton<FRolePartMeshManager>
{
public:
	FRolePartMeshManager();
	virtual ~FRolePartMeshManager();

	typedef TMap<FString, FNpcResData*>			TNpcResDataMap;
public:
	bool										LoadTable();

	/************************************************************************
	* Desc 	: ��ȡ���岿������(BODY_PART_HAIR - BODY_PART_LEG)
	* Param	: inRoeTypeID : RoleType��ID��eBodyPart : BODY_PARTö��ֵ��nPartID : ��������Ĳ���ID
	* Return	: ��Ӧ������
	* Author	: WangPeng
	* Time		: [4/23/2019 wp]
	************************************************************************/
	const FRolePartMeshData*					GetBodypartData(int32 inRoeTypeID, int32 eBodyPart,int32 nPartID);

	/************************************************************************
	* Desc 	: ��ȡ���ⲿ��������(BODY_PART_LHAND_WEAPON BODY_PART_RHAND_WEAPON BODY_PART_HORSE)
	* Param	: inRoeTypeID : RoleType��ID��eBodyPart : BODY_PARTö��ֵ��nPartID : ��������Ĳ���ID
	* Return	: ��Ӧ������
	* Author	: WangPeng
	* Time		: [4/23/2019 wp]
	************************************************************************/
	const FExtraRolePartMeshData*				GetExtraPartData(int32 inRoeTypeID, int32 eBodyPart,int32 nPartID);
private:
	bool										IsExistData(FString PartFileName);
	void										ResetData();
private:
	typedef TMap<int32, FString>				TRoleType2PartFileNameMap;
	TNpcResDataMap								m_NpcResDataMap;					//Key �����ļ����� | value ����
	TRoleType2PartFileNameMap					m_RoleType2PartFileNameMap;			//RoeTypeID ��Ӧ �����ļ����� 
};
