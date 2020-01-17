#pragma once

#include "CoreMinimal.h"

class BASEUNIT_API FNode
{
public:
	FNode* m_pNext;
	FNode* m_pPrev;

public:
	FNode(void);
	virtual ~FNode(){};
	FNode* GetNext(void);
	FNode* GetPrev(void);
	void InsertBefore(FNode* pNode);
	void InsertAfter(FNode* pNode);
	void Remove(void);
	bool IsLinked(void);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Function:	Knode
// Directions:	structure
// Parameter:	void
// Return:	void
//---------------------------------------------------------------------------
inline FNode::FNode(void)
{
	m_pNext = NULL;
	m_pPrev = NULL;
}
//---------------------------------------------------------------------------
// Function:	GetNext
// Directions:	Next node
// Parameter:	void
// Return:	KNode*
//---------------------------------------------------------------------------
inline FNode* FNode::GetNext(void)
{

	return ( m_pNext && m_pNext->m_pNext ? m_pNext : NULL );

	//if(m_pNext)
	//{
	//	if (m_pNext->m_pNext)
	//		return m_pNext;
	//}
	//
	//return NULL;
}
//---------------------------------------------------------------------------
// Function:	GetPrev
// Directions:	Previous node
// Parameter:	void
// Return:	KNode*
//---------------------------------------------------------------------------
inline FNode* FNode::GetPrev(void)
{
	if(m_pPrev)
	{
		if (m_pPrev->m_pPrev)
			return m_pPrev;
	}

	return NULL;
}
//----------------------------------------------------------------------------
// Function:	InsertBefore
// Directions:	Insert a node before
// Parameter:	KNode*
// Return:	void
//---------------------------------------------------------------------------
inline void FNode::InsertBefore(FNode *pNode)
{
	if (!pNode || !m_pPrev)
	{
		//UE_LOG(LogDataManger, Error, TEXT("Assert failed in %s, line = %i"), __FILE__, __LINE__);
		return;
	}
		

	pNode->m_pPrev = m_pPrev;
	pNode->m_pNext = this;
	m_pPrev->m_pNext = pNode;
	m_pPrev = pNode;
}
//---------------------------------------------------------------------------
// Function:	InsertAfter
// Directions:	Insert a node later
// Parameter:	KNode*
// Return:	void
//---------------------------------------------------------------------------
inline void FNode::InsertAfter(FNode *pNode)
{
	if (!pNode || !m_pNext)
	{
		//UE_LOG(LogDataManger, Error, TEXT("Assert failed in %s, line = %i"), __FILE__, __LINE__);
		return;
	}
		
	pNode->m_pPrev = this;
	pNode->m_pNext = m_pNext;
	m_pNext->m_pPrev = pNode;
	m_pNext = pNode;
}
//---------------------------------------------------------------------------
// Function:	Remove the node
// Directions:	Delete this node
// Parameter:	void
// Return:	KNode*
//---------------------------------------------------------------------------
inline void FNode::Remove(void)
{
	if (!m_pPrev || !m_pNext)
	{
		//UE_LOG(LogDataManger, Error, TEXT("Assert failed in %s, line = %i"), __FILE__, __LINE__);
	}

	if (m_pPrev)
		m_pPrev->m_pNext = m_pNext;
	if (m_pNext)
		m_pNext->m_pPrev = m_pPrev;
	m_pPrev = NULL;
	m_pNext = NULL;
}
//---------------------------------------------------------------------------
// Function:	IsLinked
// Directions:	This node is linked
// Parameter:	void
// Return:	bool
//---------------------------------------------------------------------------
inline bool FNode::IsLinked(void)
{
	return (m_pPrev && m_pNext);
}
//--------------------------------------------------------------------------------

