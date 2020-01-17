// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCObject.h"
#include "SSingleton.h"
#include "SGameInstance.h"

/*******************************������************************************
* Desc 	: ����̳�UObject�ĵ���
* Author	: WangPeng
* Time		: 2019.5.24
************************************************************************/
class OVERLORD_API FSingletonManager : public SSingleton<FSingletonManager>, public FGCObject
{
public:
	FSingletonManager();
	~FSingletonManager();

	/** Add referenced objects to stop them from GCing */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	/*********************************��������********************************/
	// Desc 	: ��ӵ���
	// Param	: T����̳�UObject
	// Return	: 
	// Author	: WangPeng
	// Time		: 2019.5.28
	/************************************************************************/
	template<typename T>
	T* AddSingleton()
	{
		UClass* pClass = T::StaticClass();
		if (!pClass)
		{
			return nullptr;
		}

		FString Name = pClass->GetName();
		if (m_SingletonMap.Contains(Name))
		{
			return nullptr;
		}

		UObject* singletonObj = NewObject<UObject>(g_pGameInstance, pClass);

		if (nullptr == singletonObj)
			return nullptr;

		m_SingletonMap.Add(Name, singletonObj);

		return Cast<T>(singletonObj);
	}

	/*********************************��������********************************/
	// Desc 	: ɾ������
	// Param	: T����̳�UObject
	// Return	: 
	// Author	: WangPeng
	// Time		: 2019.5.28
	/************************************************************************/
	template<typename T>
	void DelSingleton()
	{
		UClass* pClass = T::StaticClass();
		FString Name = pClass->GetName();
		if (m_SingletonMap.Contains(Name))
		{
			return;
		}

		m_SingletonMap.Remove(Name);
	}

	template<typename T>
	T* GetSingleton()
	{
		FString Name = T::StaticClass()->GetName();
		if (!m_SingletonMap.Contains(Name))
		{
			return nullptr;
		}

		return Cast<T>(m_SingletonMap.FindRef(Name));
	}

private:
	void OnPreGarbageCollect();

private:
	typedef TMap<FString, UObject*>		TSingletonMap;
	TSingletonMap						m_SingletonMap;
};
