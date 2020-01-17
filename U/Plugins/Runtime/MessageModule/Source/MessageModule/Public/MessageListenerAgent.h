// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DelegateMessageHandle.h"
#include "MessageDispatcher.h"
/*
*消息监听者代理
*/

#define LISTENERAGENTPOOLMAXCOUNT 200

class MESSAGEMODULE_API FMessageListenerAgent
{
public:
	FString MessageType;
	FString Filter;
	FMessageHandler Handler;

public:
	FMessageListenerAgent();
	~FMessageListenerAgent();

private:
	void Clear();
	/**********************MessageListenerAgent Pool*******************************/
private:
	static TArray<FMessageListenerAgent*> Pool;
	/*保持引用，方便游戏结束之后删除*/
	static TArray<FMessageListenerAgent*> Agents;
public:
	/*分配一个消息*/
	static FMessageListenerAgent* Allocate();
	/*释放一个消息*/
	static void Release(FMessageListenerAgent* MessageListenerAgent);
	/*在游戏退出时调用*/
	static void ShutDown();
};
