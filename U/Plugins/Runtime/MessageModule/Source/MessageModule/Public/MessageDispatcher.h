#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MessageDefine.h"
#include "Message.h"

/*
*消息监听者代理
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

	/*接收者类型*/
	typedef EMessageRecipientType::Type ERecipienType;
	typedef EMessageRecipientType::Type EOwnerType;

public:
	/*默认的接受者类型*/
	static int32 DefaultRecipientType;
	/*未得到处理的消息，这里提供一个默认的处理消息的委托*/
	static FMessageHandler MessageNotHandle;
private:
	/*保存的消息列表，直到他们被发送*/
	static TArray<FMessage*> Messages;
	/*保存委托
	*第一个层级为MessageType 第二个层级为Tag，过滤掉不需要的事件
	*/
	static TMap<FString, TMap<FString, TArray<FMessageHandler>>> MessageHandlers;

	static TArray<FMessageListenerAgent*> AgentAdds;

	static TArray<FMessageListenerAgent*> AgentRemoves;

public:
	/*清除所有的等待发送的消息队列*/
	static void ClearMessage();
	static void ClearListenners();
	/*
	*监听消息
	*@Param MessageType
	*/
	static void AddListener(FString MessageType, FMessageHandler Handler);
	static void AddListener(FString MessageType, FMessageHandler Handler, bool Immediate);
	static void AddListener(FString MessageType, FString Filter, FMessageHandler Handler, bool Immediate);
private:
	static void AddListener(FMessageListenerAgent* ListenerAgent);
public:
	/*移除消息*/
	static void RemoveListener(FString MessageType, FMessageHandler Handler);
	static void RemoveListener(FString MessageType, FMessageHandler Handler, bool Immediate);
	static void RemoveListener(FString MessageType, FString Filter, FMessageHandler Handler);
	static void RemoveListener(FString MessageType, FString Filter, FMessageHandler Handler, bool Immediate);
private:
	static void RemoveListener(FMessageListenerAgent* ListenerAgent);
public:
	/*发送消息*/
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
