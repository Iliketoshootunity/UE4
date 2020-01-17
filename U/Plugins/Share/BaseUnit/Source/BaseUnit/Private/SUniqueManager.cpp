// Fill out your copyright notice in the Description page of Project Settings.

#include "SUniqueManager.h"
#include "SLinkArray.h"

FUniqueManager::FUniqueManager()
    : m_FreeIdx(NULL)
    , m_UseIdx(NULL)
{
    
}

FUniqueManager::~FUniqueManager()
{
    if (m_FreeIdx)
    {
        delete m_FreeIdx;
        m_FreeIdx = NULL;
    }

    if (m_UseIdx)
    {
        delete m_UseIdx;
        m_UseIdx = NULL;
    }
}

void FUniqueManager::Init(int32 nSize /*= INVALID_INDEX*/)
{
    int32 TotalSize = INVALID_INDEX == nSize ? MAX_DEFAULT_UNIQUEID : nSize;

    m_FreeIdx = new FLinkArray();
    m_UseIdx = new FLinkArray();

    if (!m_FreeIdx || !m_UseIdx)
        return;

    m_FreeIdx->Init(TotalSize);
    m_UseIdx->Init(TotalSize,false);
}

void FUniqueManager::Reset(int32 nNewSize, bool bReserve /*= true*/)
{
    if (!m_FreeIdx || !m_UseIdx)
        return;

    m_FreeIdx->Reset(nNewSize, true, bReserve);
    m_UseIdx->Reset(nNewSize, false, bReserve);
}

int32 FUniqueManager::GetFreeNextID()
{
    if (!m_FreeIdx || !m_UseIdx)
        return NONE_INDEX;

    int32 index = m_FreeIdx->GetNext(0);
    if (index == NONE_INDEX)
        return NONE_INDEX;

    m_FreeIdx->RemoveAt(index);
    m_UseIdx->InsertAt(index);

    return index;
}

int32 FUniqueManager::FindFreeNextID(int32 nIdx /*= 0*/)
{
    if (!m_FreeIdx || !m_UseIdx)
        return NONE_INDEX;

    return m_FreeIdx->GetNext(nIdx);
}

int32 FUniqueManager::FindUseNextID(int32 nIdx /*= 0*/)
{
    if (!m_FreeIdx || !m_UseIdx)
        return NONE_INDEX;

    return m_UseIdx->GetNext(nIdx);
}

void FUniqueManager::FreeRemoveAndUseInsert(int32 nIdx)
{
    if (!m_FreeIdx || !m_UseIdx)
        return;

    m_FreeIdx->RemoveAt(nIdx);
    m_UseIdx->InsertAt(nIdx);
}

void FUniqueManager::UseRemoveAndFreeInsert(int32 nIdx)
{
    if (!m_FreeIdx || !m_UseIdx)
        return;

    m_UseIdx->RemoveAt(nIdx);
    m_FreeIdx->InsertAt(nIdx);
}

void FUniqueManager::FreeInsertAt(int32 nIdx)
{
	if (!m_FreeIdx || !m_UseIdx)
		return;

	m_FreeIdx->InsertAt(nIdx);
}

void FUniqueManager::UseInsertAt(int32 nIdx)
{
	if (!m_FreeIdx || !m_UseIdx)
		return;

	m_UseIdx->InsertAt(nIdx);
}

void FUniqueManager::FreeRemoveAt(int32 nIdx)
{
	if (!m_FreeIdx || !m_UseIdx)
		return;

	m_FreeIdx->RemoveAt(nIdx);
}

void FUniqueManager::UseRemoveAt(int32 nIdx)
{
	if (!m_FreeIdx || !m_UseIdx)
		return;

	m_UseIdx->RemoveAt(nIdx);
}
