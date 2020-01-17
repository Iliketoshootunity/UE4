// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GCObject.h"

class OVERLORD_API FDoubleLinkedListNode : public FGCObject
{
public:
	FDoubleLinkedListNode* PrevNode;
	FDoubleLinkedListNode* NextNode;
	UObject* Content;

	virtual void AddReferencedObjects(FReferenceCollector& Collector)  override;
};

class OVERLORD_API FDoubleLinkedList
{
public:
	FDoubleLinkedListNode* Head;
	FDoubleLinkedListNode* Tail;

protected:
	int Count;
public:
	FORCEINLINE int GetCount()
	{
		return Count;
	}
public:
	/*添加到表头*/
	FDoubleLinkedListNode* AddToHead(UObject* t);
	/*添加到表头*/
	FDoubleLinkedListNode* AddToHead(FDoubleLinkedListNode* Node);
	/*添加到表尾*/
	FDoubleLinkedListNode* AddToTail(UObject* t);

	/*添加到表尾*/
	FDoubleLinkedListNode* AddToTail(FDoubleLinkedListNode* Node);

	/*移除节点*/
	void RemoveNode(FDoubleLinkedListNode* Node);


	void MoveToHead(FDoubleLinkedListNode* Node);

};

class OVERLORD_API FMapList 
{
protected:
	FDoubleLinkedList* DoubleLinkedList;
	TMap<UObject*, FDoubleLinkedListNode*> FindMap;
public:
	FMapList();
	~FMapList();
public:
	void Clear();

	void InsertToHead(UObject* t);

	void InsertToTail(UObject* t);

	void Pop();

	void Remove(UObject* t);

	UObject* Back();

	int32 Size();

	bool Find(UObject* t);

	bool Reflesh(UObject* t);
};









