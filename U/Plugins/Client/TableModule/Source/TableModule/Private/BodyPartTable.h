// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

/************************************************************************
* Desc 	: װ��������
* Param	: 
* Return	: 
* Author	: WangPeng
* Time		: [2/28/2019 wp]
************************************************************************/

typedef struct tagBodyPartData
{
    FString PartName;	// ������
    FString BoneName;   // ������
	bool	IsExtra;	// �Ƿ��Ƕ��ⲿ�������ⲿ������Ҫ�ϲ�����ģ��
	int32   BodyPartIndex; //BODY_PART ö��ֵ
}BodyPartData;

class TABLEMODULE_API BodyPartTable : public STabBaseTable<BodyPartTable, BodyPartData>
{
public:
	BodyPartTable();
	virtual ~BodyPartTable();

public:
    virtual bool ReadTable(int32 nRow, int32& nCol) override;

	/************************************************************************
	* Desc 	: ��ȡ������Դ����
	* Param	: BodyPartName: ��������
	* Return	: 
	* Author	: WangPeng
	* Time		: [4/22/2019 wp]
	************************************************************************/
	const BodyPartData*			GetTableData(FString BodyPartName);

	/************************************************************************
	* Desc 	: ��ȡ������Դ����
	* Param	: BodyPartIndex �� BODY_PART ö��ֵ
	* Return	: 
	* Author	: WangPeng
	* Time		: [4/22/2019 wp]
	************************************************************************/
	const BodyPartData*			GetTableData(int32 BodyPartIndex);
private:
	TMap<FString, int32>		m_TName2IndexMap;
};
