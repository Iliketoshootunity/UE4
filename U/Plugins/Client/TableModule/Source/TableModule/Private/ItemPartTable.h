// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTable.h"

/************************************************************************
* Desc 	: װ�������岿λ�Ķ�Ӧ��ÿ��װ����Ӧ���3��С������װ��ö��Ϊ��BODY_PART
* Param	: 
* Return	: 
* Author	: WangPeng
* Time		: [3/1/2019 wp]
************************************************************************/

typedef struct tagItemPartData
{
    int32               EquipEnum;	        // װ��ö��
    TArray<FString>     BodyPartNameList;   // ���岿λ�������3��
}FItemPartData;

class TABLEMODULE_API FItemPartTable : public STabBaseTable < FItemPartTable, FItemPartData>
{
public:
	FItemPartTable();
	virtual ~FItemPartTable();

public:
    virtual bool ReadTable(int32 nRow, int32& nCol) override;
};
