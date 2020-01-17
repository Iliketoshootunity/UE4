#pragma once
//---------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Node.h"
//---------------------------------------------------------------------------


class BASEUNIT_API FKList
{
public:
	FNode m_ListHead; // Head node (not the first node)
	FNode m_ListTail; // Tail node (not the last node)
public:
	FKList(void);
	FNode* GetHead(void); // Get the first node
	FNode* GetTail(void); // Get the last node
	void AddHead(FNode *pNode); // Add a node to the front
	void AddTail(FNode *pNode); // Add a node at the end
	FNode* RemoveHead(void); // Delete the first node
	FNode* RemoveTail(void); // Delete the last node
	bool IsEmpty(void); // Is it an empty list
	long GetNodeCount(void);
	void Release();
};
//---------------------------------------------------------------------------

inline FKList::FKList(void)
{
	m_ListHead.m_pNext = &m_ListTail;
	m_ListTail.m_pPrev = &m_ListHead;
}

inline bool FKList::IsEmpty(void)
{
	return (m_ListHead.GetNext() == NULL);
}

inline FNode* FKList::GetHead(void)
{
	return m_ListHead.GetNext();
}
//-------------------------------------------------------------------------
// Function:	GetTail
// Directions:	Get the end of the list
// Parameter:	void
// Return:	FNode*
//---------------------------------------------------------------------------
inline FNode* FKList::GetTail(void)
{
	return m_ListTail.GetPrev();
}
//-------------------------------------------------------------------------
// Function:	AddHead
// Directions:	Add a node to the head
// Parameter:	FNode*
// Return:	BOOL
//---------------------------------------------------------------------------
inline void FKList::AddHead(FNode *pNode)
{
	m_ListHead.InsertAfter(pNode);
}
//-------------------------------------------------------------------------
// Function:	AddTail
// Directions:	Add a node at the end
// Parameter:	FNode*
// Return:	void
//---------------------------------------------------------------------------
inline void FKList::AddTail(FNode *pNode)
{
	m_ListTail.InsertBefore(pNode);
}
//-------------------------------------------------------------------------
// Function:	RemoveHead
// Directions:	Delete the first node
// Parameter:	void
// Return:	FNode*
//---------------------------------------------------------------------------
inline FNode* FKList::RemoveHead(void)
{
	FNode* pNode = m_ListHead.GetNext();
	if (pNode)
		pNode->Remove();
	return pNode;
}
//-------------------------------------------------------------------------
// Function:	RemoveTail
// Directions:	Delete the last node
// Parameter:	void
// Return:	FNode*
//---------------------------------------------------------------------------
inline FNode* FKList::RemoveTail(void)
{
	FNode* pNode = m_ListTail.GetPrev();
	if (pNode)
		pNode->Remove();
	return pNode;
}
//-------------------------------------------------------------------------
// Function:	GetNodeCount
// Directions:	
// Parameter:	void
// Return:	LONG
//---------------------------------------------------------------------------
inline long FKList::GetNodeCount(void)
{
	long nNode = 0;
	FNode* pNode = GetHead();
	while (pNode)
	{
		pNode = pNode->GetNext();
		nNode++;
	}
	return nNode;
}

inline void FKList::Release()
{
	m_ListHead.m_pNext = &m_ListTail;
	m_ListTail.m_pPrev = &m_ListHead;
}
