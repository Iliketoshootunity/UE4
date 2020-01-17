#pragma once
#include "SingletonManager.h"
#include "BasicFunction.h"

/*******************************������************************************
* Desc 	: ����ģ����
* Author	: WangPeng
* Time		: 2019.5.25
************************************************************************/

template<typename T>
class FSingletonBase
{
public:
	/*********************************��������********************************/
	// Desc 	: ȷ��һ������ʵ��������null���أ�������ָ�롣
	// Param	: ����
	// Return	: ��ʵ��ָ��
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

	/*********************************��������********************************/
	// Desc 	: ���ٵ�����������������ã��˵��������´�GCʱ�ͷš�
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