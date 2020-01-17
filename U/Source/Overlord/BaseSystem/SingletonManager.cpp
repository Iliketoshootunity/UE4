// Fill out your copyright notice in the Description page of Project Settings.

#include "SingletonManager.h"
#include "BaseSystem/GameUtil.h"

FSingletonManager::FSingletonManager()
{
	FCoreUObjectDelegates::GetPreGarbageCollectDelegate().AddRaw(this, &FSingletonManager::OnPreGarbageCollect);
}

FSingletonManager::~FSingletonManager()
{
	FCoreUObjectDelegates::GetPreGarbageCollectDelegate().RemoveAll(this);

	for (TSingletonMap::TIterator It(m_SingletonMap); It; ++It)
	{
		UGameUtil::DestroyObject(It.Value());
		It.RemoveCurrent();
	}

	FCoreUObjectDelegates::GetPreGarbageCollectDelegate().RemoveAll(this);

}

void FSingletonManager::AddReferencedObjects(FReferenceCollector& Collector)
{
	for (TSingletonMap::TConstIterator It(m_SingletonMap);It;++It)
	{
		UObject* Singleton = It.Value();
		if (Singleton)
		{
			Collector.AddReferencedObject(Singleton);
		}
	}
}

void FSingletonManager::OnPreGarbageCollect()
{
	for (TSingletonMap::TIterator It(m_SingletonMap); It; ++It)
	{
		//判断是否已经被GC，GC调后清除
		UObject* Singleton = It.Value();
		if (Singleton && !Singleton->IsValidLowLevel())
		{
			It.RemoveCurrent();
		}
	}
}
