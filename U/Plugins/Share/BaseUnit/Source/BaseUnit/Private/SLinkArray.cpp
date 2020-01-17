// Fill out your copyright notice in the Description page of Project Settings.

#include "SLinkArray.h"
#include "BaseDefine.h"

FLinkArray::FLinkArray()
{
    m_aNode.Empty();
}

FLinkArray::~FLinkArray()
{
    m_aNode.Empty();
}

FLinkNode* FLinkArray::GetNode(const int32 nIdx)
{
    if (!m_aNode.IsValidIndex(nIdx))
        return NULL;

    return &m_aNode[nIdx];
}

void FLinkArray::Init(int32 nSize, bool bInit/* = true*/)
{
    m_aNode.Empty();

    m_aNode.SetNumZeroed(nSize);

    if(bInit)
        InitLinkNode(0, m_aNode.Num());
}

void FLinkArray::RemoveAt(int32 nIdx)
{
    if (!m_aNode.IsValidIndex(nIdx))
    {
        //g_DebugLog("[error]Remove invalid node: index %d, size %d", nIdx, m_nSize);
        return;
    }

    if ((m_aNode[nIdx].nNext == 0 && m_aNode[0].nPrev != nIdx) || (m_aNode[nIdx].nPrev == 0 && m_aNode[0].nNext != nIdx))
    {
        //g_DebugLog("[error]Node:%d Remove twice", nIdx);
        return;
    }

    m_aNode[m_aNode[nIdx].nPrev].nNext = m_aNode[nIdx].nNext;
    m_aNode[m_aNode[nIdx].nNext].nPrev = m_aNode[nIdx].nPrev;
    m_aNode[nIdx].nNext = 0;
    m_aNode[nIdx].nPrev = 0;
}

void FLinkArray::InsertAt(int32 nIdx)
{
    if (!m_aNode.IsValidIndex(nIdx))
    {
        return;
    }

    if (m_aNode[nIdx].nNext != 0 || m_aNode[nIdx].nPrev != 0)
    {
        //g_DebugLog("[error]Node:%d Insert twice", nIdx);
        return;
    }

    int32 nPrevIdx = GetPrevFreeIndex(nIdx);

    m_aNode[nIdx].nNext = m_aNode[nPrevIdx].nNext;
    m_aNode[m_aNode[nPrevIdx].nNext].nPrev = nIdx;
    m_aNode[nPrevIdx].nNext = nIdx;
    m_aNode[nIdx].nPrev = nPrevIdx;

    /* m_aNode[nIdx].nNext = m_aNode[0].nNext;
     m_aNode[m_aNode[0].nNext].nPrev = nIdx;
     m_aNode[0].nNext = nIdx;
     m_aNode[nIdx].nPrev = 0;*/
}

int32 FLinkArray::GetNext(const int32 nIdx) const
{
    if (!m_aNode.IsValidIndex(nIdx))
        return NONE_INDEX;

    return m_aNode[nIdx].nNext;
}

int32 FLinkArray::GetPrev(const int32 nIdx) const
{
    if (!m_aNode.IsValidIndex(nIdx))
        return NONE_INDEX;

    return m_aNode[nIdx].nPrev;
}

void FLinkArray::Reset(int32 nNewSize, bool bInit /*= true*/, bool bReserve/* = true*/)
{
    if (m_aNode.Num() >= nNewSize)
    {
        m_aNode.Empty();
        m_aNode.SetNum(nNewSize);
        if(bInit)
            InitLinkNode(0, m_aNode.Num());
    }   
    else
    {
        if (bReserve)
        {
            int32 nPreNum = m_aNode.Num();
            m_aNode.SetNum(nNewSize);
            if (bInit)
                InitLinkNode(nPreNum, m_aNode.Num());
        }
        else
        {
            m_aNode.Empty();
            m_aNode.SetNum(nNewSize);
            if (bInit)
                InitLinkNode(0, m_aNode.Num());
        }
    } 
}

void FLinkArray::ResetInsert(int32 nIdx, int32 nFirstIdx /*= 0*/)
{
    if (!m_aNode.IsValidIndex(nIdx))
    {
        return;
    }

    if (m_aNode[nIdx].nNext != 0 || m_aNode[nIdx].nPrev != 0)
    {
        //g_DebugLog("[error]Node:%d Insert twice", nIdx);
        return;
    }

    m_aNode[nIdx].nNext = m_aNode[nFirstIdx].nNext;
    m_aNode[m_aNode[nFirstIdx].nNext].nPrev = nIdx;
    m_aNode[nFirstIdx].nNext = nIdx;
    m_aNode[nIdx].nPrev = nFirstIdx;

    if (nFirstIdx > 0 && nIdx == nFirstIdx + 1)
    {
        //必须是从大到小的循环
        int32 Index = GetPrevFreeIndex(nFirstIdx);
        m_aNode[nFirstIdx].nPrev = Index;
        m_aNode[Index].nNext = nFirstIdx;
    }
}

void FLinkArray::InitLinkNode(int32 nPrenum, int32 nMaxnum)
{
    for (int32 idx = nMaxnum - 1; idx > nPrenum; idx--)
    {
        ResetInsert(idx, nPrenum);
    }
}

int32 FLinkArray::GetPrevFreeIndex(int32 nIdx)
{
    int32 Index = nIdx - 1;
    for (int32 idx = Index; idx >= 0; idx--)
    {
        if (m_aNode[idx].nPrev == 0 && m_aNode[idx].nNext == 0)
        {
            if (m_aNode[m_aNode[idx].nPrev].nNext != idx)
            {
                continue;
            }
        }

        Index = idx;
        break;
    }

    return Index;
}

int32 FLinkArray::GetNextFreeIndex(int32 nIdx)
{
    int32 Index = nIdx + 1;
    for (int32 idx = Index; idx < m_aNode.Num(); idx++)
    {
        if (m_aNode[idx].nPrev == 0 && m_aNode[idx].nNext == 0)
        {
            if (m_aNode[m_aNode[idx].nPrev].nNext != idx)
            {
                continue;
            }
        }

        Index = idx;
        break;
    }

    return Index;
}
