#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MessageDefine.h"
#include "Message.h"

/*
*��Ϣ�����ߴ���
*/
DECLARE_DELEGATE_OneParam(FMessageHandler, FMessage*);

class FMessageListenerAgent;
class FMessage;
class UMessagedata;

class MESSAGEMODULE_API FMessageDispatcher
{
public:
	FMessageDispatcher();
	~FMessageDispatcher();

	/*����������*/
	typedef EMessageRecipientType::Type ERecipienType;
	typedef EMessageRecipientType::Type EOwnerType;

public:
	/*Ĭ�ϵĽ���������*/
	static int32 DefaultRecipientType;
	/*δ�õ��������Ϣ�������ṩһ��Ĭ�ϵĴ�����Ϣ��ί��*/
	static FMessageHandler MessageNotHandle;
private:
	/*�������Ϣ�б�ֱ�����Ǳ�����*/
	static TArray<FMessage*> Messages;
	/*����ί��
	*��һ���㼶ΪMessageType �ڶ����㼶ΪTag�����˵�����Ҫ���¼�
	*/
	static TMap<FString, TMap<FString, TArray<FMessageHandler>>> MessageHandlers;

	static TArray<FMessageListenerAgent*> AgentAdds;

	static TArray<FMessageListenerAgent*> AgentRemoves;

public:
	/*������еĵȴ����͵���Ϣ����*/
	static void ClearMessage();
	static void ClearListenners();
	/*
	*������Ϣ
	*@Param MessageType
	*/
	static void AddListener(FString MessageType, FMessageHandler Handler);
	static void AddListener(FString MessageType, FMessageHandler Handler, bool Immediate);
	static void AddListener(FString MessageType, FString Filter, FMessageHandler Handler, bool Immediate);
private:
	static void AddListener(FMessageListenerAgent* ListenerAgent);
public:
	/*�Ƴ���Ϣ*/
	static void RemoveListener(FString MessageType, FMessageHandler Handler);
	static void RemoveListener(FString MessageType, FMessageHandler Handler, bool Immediate);
	static void RemoveListener(FString MessageType, FString Filter, FMessageHandler Handler);
	static void RemoveListener(FString MessageType, FString Filter, FMessageHandler Handler, bool Immediate);
private:
	static void RemoveListener(FMessageListenerAgent* ListenerAgent);
public:
	/*������Ϣ*/
	static void SendMessage(FString MessageType);
	static void SendMessage(FString MessageType, FString Filter);
	static void SendMessage(FString MessageType, UMessagedata* MessageData);
	static void SendMessage(FString MessageType, FString Filter, UMessagedata* MessageData);
	static void SendMessage(FString MessageType, float Delay);
	static void SendMessage(FString MessageType, FString Filter, float Delay);
	static void SendMessage(void* Sender, FString Filter, FString MessageType, float Delay);
	static void SendMessage(void* Sender, FString Filter, FString MessageType, UMessagedata* MessageData, float Delay);
	static void SendMessage(void* Sender, FString Filter, FString MessageType, UMessagedata* MessageData, float Delay, EMessageRecipientType::Type RecipientType);
	static void SendMessage(FMessage* Message);
public:
	static void Tick(float D);
};
