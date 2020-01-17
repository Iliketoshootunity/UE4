// Fill out your copyright notice in the Description page of Project Settings.


#include "Message.h"
//#include "BasicFunction.h"

TArray<FMessage*> FMessage::Pool;
TArray<FMessage*> FMessage::Messages;

FMessage::FMessage()
{
	Clear();
}

FMessage::~FMessage()
{
	Clear();
}

void  FMessage::Clear(bool bDestoryData)
{
	MessageType.Empty();
	Delay = 0;
	Sender = nullptr;
	Filter = "";
	MessageData = nullptr;
	bIsSent = false;
	bIsHandled = false;
	RecipientType = EMessageRecipientType::Defalut;
}

void FMessage::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(MessageData);
}

FMessage* FMessage::Allocate()
{
	FMessage* Instance = NULL;
	if (Pool.Num() > 0)
	{
		Instance = Pool[Pool.Num() - 1];
		Pool.RemoveAt(Pool.Num() - 1);
	}
	else
	{
		Instance = new FMessage();
		//实例化由Messages管理，在ShutDown()中 释放内存
		Messages.Add(Instance);
	}
	Instance->bIsSent = false;
	Instance->bIsHandled = false;
	return Instance;

}

void FMessage::Release(FMessage* Message)
{
	if (Message == nullptr)return;
	if (Pool.Num() > MESSAGEPOOLMAXCOUNT)
	{
		delete Message;
		Message = nullptr;
		return;
	}
	Message->Clear(true);
	//表示这个消息已经完成，设置的标识符
	Message->bIsSent = true;
	Message->bIsHandled = true;
	Pool.Add(Message);
}

void FMessage::ShutDown()
{
	int Count = Messages.Num();
	for (size_t i = 0; i < Count; i++)
	{
		FMessage* Instance = Messages[Messages.Num() - 1];
		Messages.RemoveAt(Messages.Num() - 1);
		if (Instance != NULL)
		{
			delete Instance;
			Instance = NULL;
		}
		else
		{
			//FBasicFunction::Log("FMessage::ShutDown() error", true);
		}
	}
}

