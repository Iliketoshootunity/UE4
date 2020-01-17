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
	/*��ӵ���ͷ*/
	FDoubleLinkedListNode* AddToHead(UObject* t);
	/*��ӵ���ͷ*/
	FDoubleLinkedListNode* AddToHead(FDoubleLinkedListNode* Node);
	/*��ӵ���β*/
	FDoubleLinkedListNode* AddToTail(UObject* t);

	/*��ӵ���β*/
	FDoubleLinkedListNode* AddToTail(FDoubleLinkedListNode* Node);

	/*�Ƴ��ڵ�*/
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









