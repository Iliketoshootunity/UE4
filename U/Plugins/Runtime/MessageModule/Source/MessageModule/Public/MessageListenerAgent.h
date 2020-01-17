// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DelegateMessageHandle.h"
#include "MessageDispatcher.h"
/*
*��Ϣ�����ߴ���
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
	/*�������ã�������Ϸ����֮��ɾ��*/
	static TArray<FMessageListenerAgent*> Agents;
public:
	/*����һ����Ϣ*/
	static FMessageListenerAgent* Allocate();
	/*�ͷ�һ����Ϣ*/
	static void Release(FMessageListenerAgent* MessageListenerAgent);
	/*����Ϸ�˳�ʱ����*/
	static void ShutDown();
};
