#pragma once
#include "Engine/HAL/SPlatform.h"
#include "SScopeLock.h"

using namespace std;

 /*
 * 多线程Map封装
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
    //只做赋值使用
    TypeValue & operator[](TypeKey InKey)
    {
        return this->FindOrAdd(InKey);
    }

    //只做赋值使用
    const TypeValue& operator[](TypeKey InKey) const
    {
        return this->FindOrAdd(InKey);
    }

public:
    //如果存在重复Key则覆盖
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

    //如果存在重复Key则覆盖
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

    //如果存在重复Key则覆盖
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

    //如果存在重复Key则覆盖
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

    //如果存在重复Key则覆盖
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

    //如果存在重复Key则覆盖
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

    //追加key的元素，如果存在，不覆盖
    TypeValue& Append(TypeKey&& InKey)
    {
        SScopeLock Lock(&mutex);

        RPair pair = m_Data.insert(TPair(InKey, TypeValue()));
        return pair.first->second;
    }

    //追加key的元素，如果存在，不覆盖
    TypeValue& Append(TypeKey&& InKey, TypeValue&& InValue)
    {
        SScopeLock Lock(&mutex);

        RPair pair = m_Data.insert(TPair(InKey, InValue));
        return pair.first->second;
    }

    //删除key的元素
    void Remove(TypeKey&& InKey)
    {
        SScopeLock Lock(&mutex);

        m_Data.erase(InKey);
    }

    //删除key的元素
    void Remove(const TypeKey& InKey)
    {
        SScopeLock Lock(&mutex);

        m_Data.erase(InKey);
    }

    //删除元素,返回下个元素的迭代器
    TIterator Remove(TIterator InIt)
    {
        SScopeLock Lock(&mutex);

        return m_Data.erase(InIt);
    }

    //返回Key关联的值得指针
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

    //返回Key关联的值得指针
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

    //返回key关联的值，如果没有key则返回TypeValue的默认值
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

    //查找Key关联的值，如果不存在则添加
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

    //查找Key关联的值，如果不存在则添加
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

    //查找是否包含key
    bool Contains(const TypeKey& InKey) const
    {
        SScopeLock Lock(&mutex);

        return m_Data.contains(InKey);
    }
private:
    MapData m_Data;
    mutable SMutex	mutex;
};
