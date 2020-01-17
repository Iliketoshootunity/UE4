// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleLinkedList.h"


void FDoubleLinkedListNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(Content);
}

FMapList::FMapList()
{
	DoubleLinkedList = new FDoubleLinkedList();
}

FMapList::~FMapList()
{
	if (DoubleLinkedList)
	{
		delete DoubleLinkedList;
		DoubleLinkedList = nullptr;
	}
}

void FMapList::Clear()
{
	//while (DoubleLinkedList->Tail)
	//{
	//	Remove(DoubleLinkedList->Tail->Content);
	//}
}

void FMapList::InsertToHead(UObject* t)
{
	if (t == nullptr)return;
	if (FindMap.Contains(t))
	{
		if (FindMap[t] != nullptr)
		{
			DoubleLinkedList->MoveToHead(FindMap[t]);
			return;
		}
	}
	FDoubleLinkedListNode* Node = DoubleLinkedList->AddToHead(t);
	FindMap.Add(t, Node);
}

void FMapList::InsertToTail(UObject* t)
{
	if (t == nullptr)return;
	if (FindMap.Contains(t))
	{
		if (FindMap[t] != nullptr)
		{
			DoubleLinkedList->RemoveNode(FindMap[t]);
			FindMap.Remove(t);
		}
	}
	FDoubleLinkedListNode* Node = DoubleLinkedList->AddToTail(t);
	FindMap.Add(t, Node);
}

void FMapList::Pop()
{
	if (DoubleLinkedList->Tail != nullptr)
	{
		Remove(DoubleLinkedList->Tail->Content);
	}
}

void FMapList::Remove(UObject* t)
{
	if (t == nullptr)return;
	if (FindMap.Contains(t))
	{
		if (FindMap[t] != nullptr)
		{
			DoubleLinkedList->RemoveNode(FindMap[t]);
		}
	}
	FindMap.Remove(t);
}

UObject* FMapList::Back()
{
	return DoubleLinkedList->Tail != nullptr ? DoubleLinkedList->Tail->Content : nullptr;
}

int32 FMapList::Size()
{
	return FindMap.Num();
}

bool FMapList::Find(UObject* t)
{
	if (t == nullptr)return false;
	if (FindMap.Contains(t))
	{
		if (FindMap[t] != nullptr)
		{
			return true;
		}
	}
	return false;
}

bool FMapList::Reflesh(UObject* t)
{
	if (t == nullptr)return false;
	if (FindMap.Contains(t))
	{
		if (FindMap[t] != nullptr)
		{
			DoubleLinkedList->MoveToHead(FindMap[t]);
			return true;
		}
	}
	return false;
}

FDoubleLinkedListNode* FDoubleLinkedList::AddToHead(FDoubleLinkedListNode* Node)
{
	if (Node == nullptr)
	{
		return nullptr;
	}
	Node->PrevNode = nullptr;
	if (Head == nullptr)
	{
		Head = Tail = Node;
	}
	else
	{
		Head->PrevNode = Node;
		Node->NextNode = Head;
		Head = Node;
	}
	Count++;
	return Node;
}

FDoubleLinkedListNode* FDoubleLinkedList::AddToHead(UObject* t)
{
	if (t == nullptr)
	{
		return nullptr;
	}
	FDoubleLinkedListNode* temp = new FDoubleLinkedListNode();
	temp->NextNode = nullptr;
	temp->PrevNode = nullptr;
	temp->Content = t;
	return AddToHead(temp);
}

FDoubleLinkedListNode* FDoubleLinkedList::AddToTail(UObject* t)
{
	if (t == nullptr)
	{
		return nullptr;
	}
	FDoubleLinkedListNode* temp = new FDoubleLinkedListNode();
	temp->NextNode = nullptr;
	temp->PrevNode = nullptr;
	temp->Content = t;
	return AddToTail(temp);
}

FDoubleLinkedListNode* FDoubleLinkedList::AddToTail(FDoubleLinkedListNode* Node)
{
	if (Node == nullptr)
	{
		return nullptr;
	}
	Node->NextNode = nullptr;
	if (Tail == nullptr)
	{
		Head = Tail = Node;

	}
	else
	{
		Node->PrevNode = Tail;
		Tail->NextNode = Node;
		Tail = Node;
	}
	Count++;
	return Node;
}

void FDoubleLinkedList::RemoveNode(FDoubleLinkedListNode* Node)
{
	if (Node == nullptr)return;

	if (Node == Head)
	{
		Head = Node->NextNode;
	}
	if (Node == Tail)
	{
		Tail = Node->PrevNode;
	}

	if (Node->PrevNode != nullptr)
	{
		Node->PrevNode->NextNode = Node->NextNode;
	}

	if (Node->NextNode != nullptr)
	{
		Node->NextNode->PrevNode = Node->PrevNode;
	}



	Node->NextNode = nullptr;
	Node->PrevNode = nullptr;
	Node->Content = nullptr;

	delete Node;
	Node = nullptr;

	Count--;
}


void FDoubleLinkedList::MoveToHead(FDoubleLinkedListNode* Node)
{
	if (Node == nullptr || Node == Head)return;

	if (Node->PrevNode == nullptr && Node->NextNode == nullptr)return;

	if (Node == Tail)
		Tail = Node->PrevNode;

	if (Node->PrevNode)
		Node->PrevNode->NextNode = Node->NextNode;

	if (Node->NextNode)
		Node->NextNode->PrevNode = Node->PrevNode;

	Node->PrevNode = nullptr;
	Node->NextNode = Head;
	Head->PrevNode = Node;
	Head = Node;
	if (Tail == nullptr)
	{
		Tail = Head;
	}
}
