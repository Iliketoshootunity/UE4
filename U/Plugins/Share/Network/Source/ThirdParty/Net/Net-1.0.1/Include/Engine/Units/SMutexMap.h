#pragma once
#include "Engine/HAL/SPlatform.h"
#include "SScopeLock.h"

using namespace std;

 /*
 * ���߳�Map��װ
 *
 *
 */
template<typename TypeKey, typename TypeValue>
struct TMutexMap
{
public:
    typedef typename map<TypeKey, TypeValue>::iterator TIterator;
    typedef typename map<TypeKey, TypeValue>::const_iterator Const_TIterator;
    typedef pair<TypeKey, TypeValue> TPair;
    typedef pair<TIterator, bool> RPair;
    typedef map<TypeKey, TypeValue> MapData;

public:
    TMutexMap() { m_Data.empty(); }
    ~TMutexMap() { m_Data.clear(); }

    TMutexMap(TMutexMap<TypeKey, TypeValue>&& Other)
    {
        m_Data = move(Other.m_Data);
    }

    TMutexMap(const TMutexMap<TypeKey, TypeValue>& Other)
    {
        m_Data = Other.m_Data;
    }

public:
    //ֻ����ֵʹ��
    TypeValue & operator[](TypeKey InKey)
    {
        return this->FindOrAdd(InKey);
    }

    //ֻ����ֵʹ��
    const TypeValue& operator[](TypeKey InKey) const
    {
        return this->FindOrAdd(InKey);
    }

public:
    //��������ظ�Key�򸲸�
    TypeValue & Add(const TypeKey& InKey, const TypeValue& InValue)
    {
        SScopeLock Lock(&mutex);

        TIterator It = m_Data.find((TypeKey)InKey);
        if (It != m_Data.end())
        {
            It->second = InValue;
            return  It->second;
        }
        else
        {
            RPair pair = m_Data.insert(TPair(InKey, (TypeValue)InValue));
            return pair.first->second;
        }
    }

    //��������ظ�Key�򸲸�
    TypeValue& Add(const TypeKey& InKey, TypeValue&& InValue)
    {
        SScopeLock Lock(&mutex);

        TIterator It = m_Data.find((TypeKey)InKey);
        if (It != m_Data.end())
        {
            It->second = InValue;
            return  It->second;
        }
        else
        {
            RPair pair = m_Data.insert(TPair(InKey, InValue));
            return pair.first->second;
        }
    }

    //��������ظ�Key�򸲸�
    TypeValue& Add(TypeKey&& InKey, const TypeValue& InValue)
    {
        SScopeLock Lock(&mutex);

        TIterator It = m_Data.find(InKey);
        if (It != m_Data.end())
        {
            It->second = InValue;
            return  It->second;
        }
        else
        {
            RPair pair = m_Data.insert(TPair(InKey, (TypeValue)InValue));
            return pair.first->second;
        }
    }

    //��������ظ�Key�򸲸�
    TypeValue& Add(TypeKey&& InKey, TypeValue&& InValue)
    {
        SScopeLock Lock(&mutex);

        TIterator It = m_Data.find(InKey);
        if (It != m_Data.end())
        {
            It->second = InValue;
            return  It->second;
        }
        else
        {
            RPair pair = m_Data.insert(TPair(InKey, InValue));
            return pair.first->second;
        }
    }

    //��������ظ�Key�򸲸�
    TypeValue& Add(TypeKey&& InKey)
    {
        SScopeLock Lock(&mutex);

        TIterator It = m_Data.find(InKey);
        if (It != m_Data.end())
        {
            return It->second;
        }
        else
        {
            RPair pair = m_Data.insert(TPair(InKey, TypeValue()));
            return pair.first->second;
        }
    }

    //��������ظ�Key�򸲸�
    TypeValue& Add(const TypeKey& InKey)
    {
        SScopeLock Lock(&mutex);

        TIterator It = m_Data.find(InKey);
        if (It != m_Data.end())
        {
            return It->second;
        }
        else
        {
            RPair pair = m_Data.insert(TPair(InKey, TypeValue()));
            return pair.first->second;
        }
    }

    //׷��key��Ԫ�أ�������ڣ�������
    TypeValue& Append(TypeKey&& InKey)
    {
        SScopeLock Lock(&mutex);

        RPair pair = m_Data.insert(TPair(InKey, TypeValue()));
        return pair.first->second;
    }

    //׷��key��Ԫ�أ�������ڣ�������
    TypeValue& Append(TypeKey&& InKey, TypeValue&& InValue)
    {
        SScopeLock Lock(&mutex);

        RPair pair = m_Data.insert(TPair(InKey, InValue));
        return pair.first->second;
    }

    //ɾ��key��Ԫ��
    void Remove(TypeKey&& InKey)
    {
        SScopeLock Lock(&mutex);

        m_Data.erase(InKey);
    }

    //ɾ��key��Ԫ��
    void Remove(const TypeKey& InKey)
    {
        SScopeLock Lock(&mutex);

        m_Data.erase(InKey);
    }

    //ɾ��Ԫ��,�����¸�Ԫ�صĵ�����
    TIterator Remove(TIterator InIt)
    {
        SScopeLock Lock(&mutex);

        return m_Data.erase(InIt);
    }

    //����Key������ֵ��ָ��
    TypeValue* Find(TypeKey InKey)
    {
        SScopeLock Lock(&mutex);

        TIterator It = m_Data.find(InKey);
        if (It != m_Data.end())
        {
            return &It->second;
        }
 
        return (TypeValue*)NULL;
    }

    //����Key������ֵ��ָ��
    const TypeValue* Find(TypeKey InKey) const
    {
        SScopeLock Lock(&mutex);

        TIterator It = m_Data.find(InKey);
        if (It != m_Data.end())
        {
            return (const TypeValue*)&It->second;
        }

        return (const TypeValue*)NULL;
    }

    //����key������ֵ�����û��key�򷵻�TypeValue��Ĭ��ֵ
    TypeValue FindRef(TypeKey InKey)
    {
        SScopeLock Lock(&mutex);

        TIterator It = m_Data.find(InKey);
        if (It != m_Data.end())
        {
            return It->second;
        }

        return TypeValue();
    }

    MapData GetData()
    {
        SScopeLock Lock(&mutex);

        return m_Data;
    }

    //����Key������ֵ����������������
    TypeValue& FindOrAdd(TypeKey&& InKey)
    {
        SScopeLock Lock(&mutex);

        TIterator It = m_Data.find(InKey);
        if (It == m_Data.end())
        {
            RPair pair = m_Data.insert(TPair(InKey, TypeValue()));
            return pair.first->second;
        }
        else
        {
            return It->second;
        }
    }

    //����Key������ֵ����������������
    TypeValue& FindOrAdd(const TypeKey& InKey)
    {
        SScopeLock Lock(&mutex);

        Const_TIterator It = m_Data.find(InKey);
        if (It == m_Data.end())
        {
            RPair pair = m_Data.insert(TPair(InKey, TypeValue()));
            return pair.first->second;
        }
        else
        {
            return It->second;
        }
    }

    int Num()
    {
        SScopeLock Lock(&mutex);

        return (int)m_Data.size();
    }

    bool Empty()
    {
        SScopeLock Lock(&mutex);

        if (m_Data.size() == 0)
        {
            return m_Data.empty();
        }
        else
        {
            m_Data.clear();
        }

        return true;
    }

    TIterator Begin()
    {
        SScopeLock Lock(&mutex);

        return m_Data.begin();
    }

    TIterator End()
    {
        SScopeLock Lock(&mutex);

        return m_Data.end();
    }

    //�����Ƿ����key
    bool Contains(const TypeKey& InKey) const
    {
        SScopeLock Lock(&mutex);

        return m_Data.contains(InKey);
    }
private:
    MapData m_Data;
    mutable SMutex	mutex;
};
