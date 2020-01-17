#pragma once
#include "SingletonManager.h"
#include "BasicFunction.h"

/*******************************类声明************************************
* Desc 	: 单例模板类
* Author	: WangPeng
* Time		: 2019.5.25
************************************************************************/

template<typename T>
class FSingletonBase
{
public:
	/*********************************函数声明********************************/
	// Desc 	: 确保一定存在实例，避免null返回，否则会空指针。
	// Param	: 类名
	// Return	: 类实例指针
	// Author	: WangPeng
	// Time		: 2019.5.25
	/************************************************************************/
	static T* Get()
	{
		T* p = FSingletonManager::Get()->GetSingleton<T>();
		if (p == nullptr)
		{
			p = FSingletonManager::Get()->AddSingleton<T>();
			if (p == nullptr)
			{
				FBasicFunction::Logs(TEXT("Error : AddSingleton Class Name Is %s Failed!!"),*T::StaticClass()->GetName());
			}
		}

		return p;
	}

	/*********************************函数声明********************************/
	// Desc 	: 销毁单例，清除单例的引用，此单例将在下次GC时释放。
	// Param	: 
	// Return	: 
	// Author	: WangPeng
	// Time		: 2019.5.28
	/************************************************************************/
	static void Destroy()
	{
		FSingletonManager::Get()->DelSingleton<T>();
	}
};