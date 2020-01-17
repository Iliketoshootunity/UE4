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
	FLinkArray*                     m_FreeIdx;      //可用表
	FLinkArray*                     m_UseIdx;       //已用表

public:
	FUniqueManager();
	~FUniqueManager();

    void                            Init(int32 nSize = INVALID_INDEX);
    void                            Reset(int32 nNewSize, bool bReserve = true);

    /************************************************************************
    * Desc 	: 获取未使用的唯一ID,此函数成功的话，会直接remove free表的index，并Add到user表里
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/8/2019 wp]
    ************************************************************************/
    int32                           GetFreeNextID();

    /************************************************************************
    * Desc 	: 查找未使用的唯一ID，仅查询使用
    * Param	: 返回内存0位置上的数据
    * Return	:
    * Author	: WangPeng
    * Time		: [3/8/2019 wp]
    ************************************************************************/
    int32                           FindFreeNextID(int32 nIdx = 0);

    /************************************************************************
    * Desc 	: 查找已使用的唯一ID，仅查询使用
    * Param	: 返回内存0位置上的数据
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/8/2019 wp]
    ************************************************************************/
    int32                           FindUseNextID(int32 nIdx = 0);

    /************************************************************************
    * Desc 	: 未用表清除，已用表插入
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/9/2019 wp]
    ************************************************************************/
    void                            FreeRemoveAndUseInsert(int32 nIdx);

    /************************************************************************
    * Desc 	: 已用表清除，未用表插入
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/9/2019 wp]
    ************************************************************************/
    void                            UseRemoveAndFreeInsert(int32 nIdx);


	
	/////////////////////////以下函数，并非单独执行，否则数据出现丢失
	/************************************************************************
	* Desc 	: 未用表插入, 函数调用后，需要调用UseRemoveAt，保持表数据统一
	* Param	:
	* Return	:
	* Author	: WangPeng
	* Time		: [3/9/2019 wp]
	************************************************************************/
	void                            FreeInsertAt(int32 nIdx);

	/************************************************************************
	* Desc 	: 已用表插入，调用此函数前需要调用FreeInsertAt，保持数据统一
	* Param	:
	* Return	:
	* Author	: WangPeng
	* Time		: [3/9/2019 wp]
	************************************************************************/
	void                            UseInsertAt(int32 nIdx);

	/************************************************************************
	* Desc 	: 未用表清除，函数调用后，需要调用UseInsertAt，保持表数据统一
	* Param	:
	* Return	:
	* Author	: WangPeng
	* Time		: [3/9/2019 wp]
	************************************************************************/
	void                            FreeRemoveAt(int32 nIdx);

	/************************************************************************
	* Desc 	: 已用表清除，函数调用后，需要调用FreeInsertAt，保持表数据统一
	* Param	:
	* Return	:
	* Author	: WangPeng
	* Time		: [3/9/2019 wp]
	************************************************************************/
	void                            UseRemoveAt(int32 nIdx);

};
