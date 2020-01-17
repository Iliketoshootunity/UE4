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
	/*������*/
	void* Sender;
	/*���˱�ʶ*/
	FString Filter = "";
	/*�ӳٶ����뷢��*/
	float Delay;
	/*���͵�����*/
	UMessagedata* MessageData;
	/*�Ƿ���*/
	bool bIsSent;
	/*�Ƿ����������Ϣ�����������ӳٷ��ͣ�δ����,�������Ҳ���ڴ�������Ϣ*/
	bool bIsHandled;
	/*����������*/
	typedef EMessageRecipientType::Type _RecipienType;
	_RecipienType RecipientType;

	virtual void Clear(bool bDestoryData = false);

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	/**********************Message Pool*******************************/
private:
	static TArray<FMessage*> Pool;
	/*�������ã�������Ϸ����֮��ɾ��*/
	static TArray<FMessage*> Messages;
public:
	/*����һ����Ϣ*/
	static FMessage* Allocate();
	/*�ͷ�һ����Ϣ*/
	static void Release(FMessage* Message);
	/*����Ϸ�˳�ʱ����*/
	static void ShutDown();
};


