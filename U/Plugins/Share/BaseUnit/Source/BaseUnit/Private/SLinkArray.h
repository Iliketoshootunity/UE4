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
    * Desc 	:  ʹ�ù�����RemoveAt�������ʱ���ô˺���������ʱ��ʹ�û�������ݴ���
    * Param	:  ���ݲ���λ��
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/8/2019 wp]
    ************************************************************************/
    void			InsertAt(int32 nIdx);   
    int32			GetNext(const int32 nIdx) const;
    int32			GetPrev(const int32 nIdx) const;

    //��������nNewSize <= ԭ����С��һ�����ò�����ԭ�����ݣ�bReserve = true����ԭ�����ݣ�=false��������ԭ������
    void            Reset(int32 nNewSize, bool bInit = true, bool bReserve = true);

private: 
    void            ResetInsert(int32 nIdx, int32 nFirstIdx = 0);
    void            InitLinkNode(int32 nPrenum,int32 nMaxnum);

    //��ȡ��ǰnIdxǰ�棬��һ��δʹ�õı�����
    int32           GetPrevFreeIndex(int32 nIdx);
    //��ȡ��ǰnIdx���棬��һ��δʹ�õı�����
    int32           GetNextFreeIndex(int32 nIdx);
};
