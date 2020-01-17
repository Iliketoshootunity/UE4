
#include "MessageDispatcher.h"
#include "Message.h"
#include "MessageListenerAgent.h"
#include "Engine/Classes/GameFramework/Actor.h"
//#include "BasicFunction.h"


int FMessageDispatcher::DefaultRecipientType = EMessageRecipientType::Defalut;
FMessageHandler FMessageDispatcher::MessageNotHandle;
TArray<FMessage*> FMessageDispatcher::Messages = TArray<FMessage*>();
TMap<FString, TMap<FString, TArray<FMessageHandler>>> FMessageDispatcher::MessageHandlers = TMap<FString, TMap<FString, TArray<FMessageHandler>>>();
TArray<FMessageListenerAgent*> FMessageDispatcher::AgentAdds = TArray<FMessageListenerAgent*>();
TArray<FMessageListenerAgent*> FMessageDispatcher::AgentRemoves = TArray<FMessageListenerAgent*>();



FMessageDispatcher::FMessageDispatcher()
{
}

FMessageDispatcher::~FMessageDispatcher()
{

}

void FMessageDispatcher::ClearMessage()
{
	Messages.Empty();
}

void FMessageDispatcher::ClearListenners()
{
	MessageHandlers.Empty();
	AgentAdds.Empty();
	AgentRemoves.Empty();
}

void FMessageDispatcher::AddListener(FString MessageType, FMessageHandler Handler)
{
	AddListener(MessageType, "Unity3DAndUE4", Handler, false);
}

void FMessageDispatcher::AddListener(FString MessageType, FMessageHandler Handler, bool Immediate)
{
	AddListener(MessageType, "Unity3DAndUE4", Handler, Immediate);
}


void FMessageDispatcher::AddListener(FString MessageType, FString Filter, FMessageHandler Handler, bool Immediate)
{
	FMessageListenerAgent* Agent = FMessageListenerAgent::Allocate();
	Agent->MessageType = MessageType;
	Agent->Filter = Filter;
	Agent->Handler = Handler;
	if (Immediate)
	{
		AddListener(Agent);
		FMessageListenerAgent::Release(Agent);
	}
	else
	{
		AgentAdds.Add(Agent);
	}
}

void FMessageDispatcher::AddListener(FMessageListenerAgent* ListenerAgent)
{
	if (ListenerAgent == nullptr)return;
	TMap<FString, TArray<FMessageHandler>> FilterMap;
	if (MessageHandlers.Contains(ListenerAgent->MessageType))
	{
		FilterMap = MessageHandlers[ListenerAgent->MessageType];
	}
	else
	{
		//Add 会运用RecipienMap 重新构造一个实例添加到MessageHandlers末尾
		MessageHandlers.Add(ListenerAgent->MessageType, FilterMap);
	}

	//如果没有这个过滤器的委托列表，创建它
	if (!FilterMap.Contains(ListenerAgent->Filter))
	{
		TArray<FMessageHandler> NewMessageArr;
		FilterMap.Add(ListenerAgent->Filter, NewMessageArr);
		MessageHandlers[ListenerAgent->MessageType].Add(ListenerAgent->Filter, NewMessageArr);
	}


	//添加消息
	MessageHandlers[ListenerAgent->MessageType][ListenerAgent->Filter].Add(ListenerAgent->Handler);

	//释放消息代理
	FMessageListenerAgent::Release(ListenerAgent);

}

void FMessageDispatcher::RemoveListener(FString MessageType, FMessageHandler Handler)
{
	RemoveListener(MessageType, "Unity3DAndUE4", Handler, true);
}

void FMessageDispatcher::RemoveListener(FString MessageType, FMessageHandler Handler, bool Immediate)
{
	RemoveListener(MessageType, "Unity3DAndUE4", Handler, Immediate);
}

void FMessageDispatcher::RemoveListener(FString MessageType, FString Filter, FMessageHandler Handler)
{
	RemoveListener(MessageType, Filter, Handler, true);
}

void FMessageDispatcher::RemoveListener(FString MessageType, FString Filter, FMessageHandler Handler, bool Immediate)
{
	FMessageListenerAgent* Agent = FMessageListenerAgent::Allocate();
	Agent->MessageType = MessageType;
	Agent->Filter = Filter;
	Agent->Handler = Handler;
	if (Immediate)
	{
		RemoveListener(Agent);
		FMessageListenerAgent::Release(Agent);
	}
	else
	{
		AgentRemoves.Add(Agent);
	}
}

void FMessageDispatcher::RemoveListener(FMessageListenerAgent* ListenerAgent)
{
	if (ListenerAgent == nullptr)return;
	if (MessageHandlers.Contains(ListenerAgent->MessageType))
	{
		if (MessageHandlers[ListenerAgent->MessageType].Contains(ListenerAgent->Filter))
		{
			int32 FilterCount = MessageHandlers[ListenerAgent->MessageType][ListenerAgent->Filter].Num();
			for (int32 i = FilterCount - 1; i >= 0; i--)
			{
				bool IsOk = false;
				//判断两个委托是否相等
				FMessageHandler& SaveHandler = MessageHandlers[ListenerAgent->MessageType][ListenerAgent->Filter][i];
				FMessageHandler& AgentHandler = ListenerAgent->Handler;
				uint64 A1 = SaveHandler.GetBoundProgramCounterForTimerManager();
				uint64 A2 = AgentHandler.GetBoundProgramCounterForTimerManager();
				const void* v1 = SaveHandler.GetObjectForTimerManager();
				const void* v2 = AgentHandler.GetObjectForTimerManager();
				if (v1 == v2 && A1 == A2)
				{
					IsOk = true;
				}
				if (IsOk)
				{
					//移除这个委托
					FMessageHandler& Handler = MessageHandlers[ListenerAgent->MessageType][ListenerAgent->Filter][i];
					Handler.Unbind();
					MessageHandlers[ListenerAgent->MessageType][ListenerAgent->Filter].RemoveAt(i);
				}
			}
			int32 NewFilterCount = MessageHandlers[ListenerAgent->MessageType][ListenerAgent->Filter].Num();
			if (NewFilterCount <= 0)
			{
				MessageHandlers[ListenerAgent->MessageType].Remove(ListenerAgent->Filter);
				if (MessageHandlers[ListenerAgent->MessageType].Num() <= 0)
				{
					MessageHandlers.Remove(ListenerAgent->MessageType);
				}
			}
		}
	}

	//释放代理
	FMessageListenerAgent::Release(ListenerAgent);

}

void FMessageDispatcher::SendMessage(FString MessageType)
{
	SendMessage(MessageType, "Unity3DAndUE4");
}

void FMessageDispatcher::SendMessage(FString MessageType, FString Filter)
{
	SendMessage(nullptr, "Unity3DAndUE4", MessageType, nullptr, 0, EMessageRecipientType::Defalut);
}

void FMessageDispatcher::SendMessage(FString MessageType, UMessagedata* MessageData)
{
	SendMessage(nullptr, "Unity3DAndUE4", MessageType, MessageData, 0, EMessageRecipientType::Defalut);
}

void FMessageDispatcher::SendMessage(FString MessageType, FString Filter, UMessagedata* MessageData)
{
	SendMessage(nullptr, Filter, MessageType, MessageData, 0, EMessageRecipientType::Defalut);
}

void FMessageDispatcher::SendMessage(FString MessageType, float Delay)
{
	SendMessage(MessageType, "Unity3DAndUE4", Delay);
}

void FMessageDispatcher::SendMessage(FString MessageType, FString Filter, float Delay)
{
	SendMessage(nullptr, Filter, MessageType, Delay);
}

void FMessageDispatcher::SendMessage(void* Sender, FString Filter, FString MessageType, float Delay)
{
	SendMessage(Sender, Filter, MessageType, nullptr, Delay);
}

void FMessageDispatcher::SendMessage(void* Sender, FString Filter, FString MessageType, UMessagedata* MessageData, float Delay)
{
	SendMessage(Sender, Filter, MessageType, MessageData, Delay, EMessageRecipientType::Defalut);
}
void FMessageDispatcher::SendMessage(void* Sender, FString Filter, FString MessageType, UMessagedata* MessageData, float Delay, EMessageRecipientType::Type RecipientType)
{
	//构建消息
	FMessage* Message = FMessage::Allocate();
	Message->Sender = Sender;
	Message->Filter = Filter;
	Message->MessageType = MessageType;
	Message->MessageData = MessageData;
	Message->Delay = Delay;
	Message->RecipientType = RecipientType;
	//如果VoidRefDatas有参数，强制立即发送
	if (MessageData->VoidRefDatas.Num() > 0)
	{
		Delay = 0;
	}
	//发送消息
	SendMessage(Message);
	//如果时即时发送的，用完了消息，下一步释放它
	if (Delay == 0)
	{
		FMessage::Release(Message);
	}
}

void FMessageDispatcher::SendMessage(FMessage* Message)
{
	//是否没有接收者
	bool ReportMissingRecipient = true;
	/*>0 说明延迟发送的时间,<0说明下一帧发送 */
	if (Message->Delay > 0 || Message->Delay < 0)
	{
		if (!Messages.Contains(Message))
		{
			Messages.Add(Message);
		}
		ReportMissingRecipient = false;
	}
	else if (MessageHandlers.Contains(Message->MessageType))
	{
		for (TMap<FString, TArray<FMessageHandler>>::TIterator It(MessageHandlers[Message->MessageType]); It; ++It)
		{
			bool bExecuteHandler = false;
			if (It.Value().Num() == 0)
			{
				//没有任何消息
				continue;
			}
			//满足过滤条件
			if (It.Key() == Message->Filter)
			{
				bExecuteHandler = true;
				Message->bIsSent = true;
			}
			if (bExecuteHandler)
			{
				Message->bIsSent = true;
				//循环执行所有的消息
				int32 Count = It.Value().Num();
				for (int32 i = 0; i < Count; i++)
				{
					It.Value()[i].ExecuteIfBound(Message);
				}
				ReportMissingRecipient = false;
				return;
			}
		}
	}

	if (ReportMissingRecipient)
	{
		if (MessageNotHandle.IsBound() == false)
		{
			//FBasicFunction::Log("Message Match Failed,and MessageNotHandle is null ");
		}
		else
		{
			//MessageNotHandle.ExecuteIfBound(Message);
		}
		Message->bIsSent = true;
	}

}

void FMessageDispatcher::Tick(float D)
{

	int32 MessageCount = Messages.Num();
	if (MessageCount > 0)
	{
		for (int32 i = MessageCount - 1; i >= 0; i--)
		{
			FMessage* m = Messages[i];
			if (m)
			{
				if (!m->bIsSent)
				{
					m->Delay -= D;
					if (m->Delay < 0)
					{
						m->Delay = 0;
						SendMessage(m);
						Messages.Remove(m);
						FMessage::Release(m);
					}
				}
				else
				{
					//FBasicFunction::Log("FMessageDispatcher::Tick FMessage was send");
				}

			}
		}

	}

	if (AgentRemoves.Num() > 0)
	{
		for (int32 i = AgentRemoves.Num() - 1; i >= 0; i--)
		{
			RemoveListener(AgentRemoves[i]);
		}

		AgentRemoves.Empty();
	}


	for (int32 i = 0; i < AgentAdds.Num(); i++)
	{
		AddListener(AgentAdds[i]);
	}

	AgentAdds.Empty();

}
