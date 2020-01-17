// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
typedef struct tagLinkNode
{
public:
    uint32  nPrev;
    uint32	nNext;
public:
    tagLinkNode() { nPrev = nNext = 0; }

}FLinkNode;

class BASEUNIT_API FLinkArray
{
private:
    TArray<FLinkNode>   m_aNode;

public:
	FLinkArray();
	~FLinkArray();

    FLinkNode		*GetNode(const int32 nIdx);
    

    void            Init(int32 nSize,bool bInit = true);
    void			RemoveAt(int32 nIdx);

    /************************************************************************
    * Desc 	:  使用规则，在RemoveAt后再添加时调用此函数，其他时候使用会出现数据错乱
    * Param	:  数据插入位置
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/8/2019 wp]
    ************************************************************************/
    void			InsertAt(int32 nIdx);   
    int32			GetNext(const int32 nIdx) const;
    int32			GetPrev(const int32 nIdx) const;

    //重置链表nNewSize <= 原来大小则一律重置不保留原来数据，bReserve = true则保留原来数据，=false，不保留原来数据
    void            Reset(int32 nNewSize, bool bInit = true, bool bReserve = true);

private: 
    void            ResetInsert(int32 nIdx, int32 nFirstIdx = 0);
    void            InitLinkNode(int32 nPrenum,int32 nMaxnum);

    //获取当前nIdx前面，第一个未使用的表索引
    int32           GetPrevFreeIndex(int32 nIdx);
    //获取当前nIdx后面，第一个未使用的表索引
    int32           GetNextFreeIndex(int32 nIdx);
};
