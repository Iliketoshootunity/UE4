#include "MessageListenerAgent.h"

TArray<FMessageListenerAgent*> FMessageListenerAgent::Pool;
TArray<FMessageListenerAgent*> FMessageListenerAgent::Agents;

FMessageListenerAgent::FMessageListenerAgent()
{
	Clear();
}

FMessageListenerAgent::~FMessageListenerAgent()
{
	Clear();
}

void FMessageListenerAgent::Clear()
{
	MessageType.Empty();
	Filter.Empty();
	Handler.Unbind();
}

FMessageListenerAgent* FMessageListenerAgent::Allocate()
{
	FMessageListenerAgent* Instance = NULL;
	if (Pool.Num() > 0)
	{
		Instance = Pool[Pool.Num() - 1];
		Pool.RemoveAt(Pool.Num() - 1);
	}
	else
	{
		Instance = new FMessageListenerAgent();
		//实例化由Agents管理，在ShutDown()中 释放内存
		Agents.Add(Instance);
	}
	return Instance;
}

void FMessageListenerAgent::Release(FMessageListenerAgent* MessageListenerAgent)
{
	if (MessageListenerAgent == nullptr)return;
	if (Pool.Num() > LISTENERAGENTPOOLMAXCOUNT)
	{
		delete MessageListenerAgent;
		MessageListenerAgent = nullptr;
		return;
	}
	MessageListenerAgent->Clear();
	if (Pool.Contains(MessageListenerAgent))return;
	Pool.Add(MessageListenerAgent);
}
void FMessageListenerAgent::ShutDown()
{
	int Count = Agents.Num();
	for (size_t i = 0; i < Count; i++)
	{
		FMessageListenerAgent* Instance = Agents[Agents.Num() - 1];
		Agents.RemoveAt(Agents.Num() - 1);
		if (Instance != NULL)
		{
			delete Instance;
			Instance = NULL;
		}
	}
}