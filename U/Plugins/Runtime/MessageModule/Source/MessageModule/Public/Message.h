// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MessageDefine.h"
#include "GCObject.h"
#include "MessageData.h"

#define MESSAGEPOOLMAXCOUNT 200

/**
 *
 */
class MESSAGEMODULE_API FMessage : public FGCObject
{
public:
	FMessage();
	virtual ~FMessage();
public:
	/**/
	FString MessageType;
	/*发送者*/
	void* Sender;
	/*过滤标识*/
	FString Filter = "";
	/*延迟多少秒发送*/
	float Delay;
	/*发送的数据*/
	UMessagedata* MessageData;
	/*是否发送*/
	bool bIsSent;
	/*是否处理了这个消息，可能属于延迟发送，未发送,这种情况也属于处理了消息*/
	bool bIsHandled;
	/*接收者类型*/
	typedef EMessageRecipientType::Type _RecipienType;
	_RecipienType RecipientType;

	virtual void Clear(bool bDestoryData = false);

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	/**********************Message Pool*******************************/
private:
	static TArray<FMessage*> Pool;
	/*保持引用，方便游戏结束之后删除*/
	static TArray<FMessage*> Messages;
public:
	/*分配一个消息*/
	static FMessage* Allocate();
	/*释放一个消息*/
	static void Release(FMessage* Message);
	/*在游戏退出时调用*/
	static void ShutDown();
};


