// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefine.h"

/**
 * 
 */
class FLinkArray;

class BASEUNIT_API FUniqueManager
{
private:
	FLinkArray*                     m_FreeIdx;      //���ñ�
	FLinkArray*                     m_UseIdx;       //���ñ�

public:
	FUniqueManager();
	~FUniqueManager();

    void                            Init(int32 nSize = INVALID_INDEX);
    void                            Reset(int32 nNewSize, bool bReserve = true);

    /************************************************************************
    * Desc 	: ��ȡδʹ�õ�ΨһID,�˺����ɹ��Ļ�����ֱ��remove free���index����Add��user����
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/8/2019 wp]
    ************************************************************************/
    int32                           GetFreeNextID();

    /************************************************************************
    * Desc 	: ����δʹ�õ�ΨһID������ѯʹ��
    * Param	: �����ڴ�0λ���ϵ�����
    * Return	:
    * Author	: WangPeng
    * Time		: [3/8/2019 wp]
    ************************************************************************/
    int32                           FindFreeNextID(int32 nIdx = 0);

    /************************************************************************
    * Desc 	: ������ʹ�õ�ΨһID������ѯʹ��
    * Param	: �����ڴ�0λ���ϵ�����
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/8/2019 wp]
    ************************************************************************/
    int32                           FindUseNextID(int32 nIdx = 0);

    /************************************************************************
    * Desc 	: δ�ñ���������ñ����
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/9/2019 wp]
    ************************************************************************/
    void                            FreeRemoveAndUseInsert(int32 nIdx);

    /************************************************************************
    * Desc 	: ���ñ������δ�ñ����
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/9/2019 wp]
    ************************************************************************/
    void                            UseRemoveAndFreeInsert(int32 nIdx);


	
	/////////////////////////���º��������ǵ���ִ�У��������ݳ��ֶ�ʧ
	/************************************************************************
	* Desc 	: δ�ñ����, �������ú���Ҫ����UseRemoveAt�����ֱ�����ͳһ
	* Param	:
	* Return	:
	* Author	: WangPeng
	* Time		: [3/9/2019 wp]
	************************************************************************/
	void                            FreeInsertAt(int32 nIdx);

	/************************************************************************
	* Desc 	: ���ñ���룬���ô˺���ǰ��Ҫ����FreeInsertAt����������ͳһ
	* Param	:
	* Return	:
	* Author	: WangPeng
	* Time		: [3/9/2019 wp]
	************************************************************************/
	void                            UseInsertAt(int32 nIdx);

	/************************************************************************
	* Desc 	: δ�ñ�������������ú���Ҫ����UseInsertAt�����ֱ�����ͳһ
	* Param	:
	* Return	:
	* Author	: WangPeng
	* Time		: [3/9/2019 wp]
	************************************************************************/
	void                            FreeRemoveAt(int32 nIdx);

	/************************************************************************
	* Desc 	: ���ñ�������������ú���Ҫ����FreeInsertAt�����ֱ�����ͳһ
	* Param	:
	* Return	:
	* Author	: WangPeng
	* Time		: [3/9/2019 wp]
	************************************************************************/
	void                            UseRemoveAt(int32 nIdx);

};
