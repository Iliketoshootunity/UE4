// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUnitManager.h"
#include "SSingleton.h"
#include "BaseDefine.h"
#include "TableModuleLog.h"



/************************************************************************
* Desc 	: Tab文件单例类读表基类
* Param	: T :类名； D ：数据结构体名 ； V 存储Key的类型
* Return	: 
* Author	: WangPeng
* Time		: [3/1/2019 wp]
************************************************************************/
template<class T, typename D, typename V = int32>
class TABLEMODULE_API STabBaseTable : public SSingleton<T>
{
    GENERATED_SUPER(STabBaseTable)
public:
    STabBaseTable(T* pThis) :m_pThis(pThis), m_bIsLoaded(false) { m_FileName.Empty(); m_DataLst.Empty(); }
    STabBaseTable(T* pThis, FString FileName) :m_pThis(pThis), m_FileName(FileName), m_bIsLoaded(false) { m_DataLst.Empty(); }

    typedef TMap<V, D> FDataMap;

    virtual ~STabBaseTable()
    {
        m_FileName.Empty();
        m_pThis = NULL;
        m_bIsLoaded = false;
        m_DataLst.Empty();
    }

public:
    virtual bool                                    LoadTable()
    {
        if (!m_pThis || m_FileName.IsEmpty())
            return false;

        m_bIsLoaded = false;
        m_DataLst.Empty();

        if (!m_TableManager.LoadTab(m_FileName))
        {
            Logs(TEXT("load %s failed!"), true, 20.f, FColor::Red, *m_FileName);
            m_TableManager.UnLoadTab();
            return false;
        }

        int32 nRecordCount = m_TableManager.GetTabHeight();
        int32 nRow = m_TableManager.GetBeginRow(); //从第三行开始
        for (int32 nIndex = 0; nIndex < nRecordCount; ++nIndex)
        {
            int32	nCol = 1;
            ++nRow;

            if (!((T*)m_pThis)->ReadTable(nRow, nCol))
            {
                Logs(TEXT("ReadTable %s [ Row:%d , Col:%d ]failed!"), true, 20.f, FColor::Red, *m_FileName, nRow, nCol);
                m_TableManager.UnLoadTab();
                return false;
            }
        }
        m_TableManager.UnLoadTab();

        m_bIsLoaded = true;
        return true;
    }

    virtual bool                                    LoadTable(FString FileName)
    {
        if (!m_pThis || FileName.IsEmpty())
            return false;

        m_bIsLoaded = false;
        m_DataLst.Empty();

        if (!m_TableManager.LoadTab(FileName))
        {
            Logs(TEXT("load %s failed!"), true, 20.f, FColor::Red, *FileName);
            m_TableManager.UnLoadTab();
            return false;
        }

        int32 nRecordCount = m_TableManager.GetTabHeight();
        int32 nRow = m_TableManager.GetBeginRow(); //从第三行开始
        for (int32 nIndex = 0; nIndex < nRecordCount; ++nIndex)
        {
            int32	nCol = 1;
            ++nRow;

            if (!((T*)m_pThis)->ReadTable(nRow, nCol))
            {
                Logs(TEXT("ReadTable %s [ Row:%d , Col:%d ]failed!"), true, 20.f, FColor::Red, *FileName, nRow, nCol);
                m_TableManager.UnLoadTab();
                return false;
            }
        }
        m_TableManager.UnLoadTab();

        m_FileName = FileName;
        m_bIsLoaded = true;
        return true;
    }

    virtual bool                                    ReadTable(int32 nRow, int32& nCol) = 0;

protected:
    virtual FORCEINLINE void                        SetData(V key, D data)
    {
        m_DataLst.Add(key, data);
    }

public:
    virtual FORCEINLINE int32                       GetDataNum()
    {
        return m_DataLst.Num();
    }

    virtual FORCEINLINE const D*                    GetData(V key)
    {
        return m_DataLst.Find(key);
    }

    virtual FORCEINLINE const D                     GetDataRef(V key)
    {
        return m_DataLst.FindRef(key);
    }

    virtual FORCEINLINE FDataMap*                   GetData()
    {
        return &m_DataLst;
    }

public:
    FORCEINLINE bool                                IsLoaded()
    {
        return m_bIsLoaded;
    }

    FORCEINLINE FString                             GetFileName()
    {
        return m_FileName;
    }

    FORCEINLINE BaseUnitManager*                    GetTableManager()
    {
        return &m_TableManager;
    }

    FORCEINLINE bool                                IsValid(V key)
    {
        return m_DataLst.Contains(key);
    }

private:
    T *                                             m_pThis;
    FDataMap                                        m_DataLst;
    FString                                         m_FileName;
    bool                                            m_bIsLoaded;

    BaseUnitManager                                 m_TableManager;
};


/*
* Tab文件非单例表基类 multi
*/

template<class T, typename D, typename V = int32>
class TABLEMODULE_API MTabBaseTable
{
    GENERATED_SUPER(MTabBaseTable)
public:
    MTabBaseTable(T* pThis) :m_pThis(pThis), m_bIsLoaded(false) { m_FileName.Empty(); m_DataLst.Empty(); }
    MTabBaseTable(T* pThis, FString FileName) :m_pThis(pThis), m_FileName(FileName), m_bIsLoaded(false) { m_DataLst.Empty(); }

    typedef TMap<V, D> FDataMap;

    virtual ~MTabBaseTable()
    {
        m_FileName.Empty();
        m_pThis = NULL;
        m_bIsLoaded = false;
        m_DataLst.Empty();
    }

public:
    virtual bool                            LoadTable()
    {
        if (!m_pThis || m_FileName.IsEmpty())
            return false;

        m_bIsLoaded = false;
        m_DataLst.Empty();

        if (!m_TableManager.LoadTab(m_FileName))
        {
            Logs(TEXT("load %s failed!"), true, 20.f, FColor::Red, *m_FileName);
            m_TableManager.UnLoadTab();
            return false;
        }

        int32 nRecordCount = m_TableManager.GetTabHeight();
        int32 nRow = m_TableManager.GetBeginRow(); //从第三行开始
        for (int32 nIndex = 0; nIndex < nRecordCount; ++nIndex)
        {
            int32	nCol = 1;
            ++nRow;

            if (!((T*)m_pThis)->ReadTable(nRow, nCol))
            {
                Logs(TEXT("load %s [ Row:%d , Col:%d ]failed!"), true, 20.f, FColor::Red, *m_FileName, nRow, nCol);
                m_TableManager.UnLoadTab();
                return false;
            }
        }
        m_TableManager.UnLoadTab();

        m_bIsLoaded = true;
        return true;
    }

    virtual bool                            LoadTable(FString FileName)
    {
        if (!m_pThis || FileName.IsEmpty())
            return false;

        m_bIsLoaded = false;
        m_DataLst.Empty();

        if (!m_TableManager.LoadTab(FileName))
        {
            Logs(TEXT("load %s failed!"), true, 20.f, FColor::Red, *FileName);
            m_TableManager.UnLoadTab();
            return false;
        }

        int32 nRecordCount = m_TableManager.GetTabHeight();
        int32 nRow = m_TableManager.GetBeginRow(); //从第三行开始
        for (int32 nIndex = 0; nIndex < nRecordCount; ++nIndex)
        {
            int32	nCol = 1;
            ++nRow;

            if (!((T*)m_pThis)->ReadTable(nRow, nCol))
            {
                Logs(TEXT("load %s [ Row:%d , Col:%d ]failed!"), true, 20.f, FColor::Red, *FileName, nRow, nCol);
                m_TableManager.UnLoadTab();
                return false;
            }
        }
        m_TableManager.UnLoadTab();
        m_FileName = FileName;
        m_bIsLoaded = true;
        return true;
    }

    virtual bool                            ReadTable(int32 nRow, int32& nCol) = 0;

protected:
    virtual FORCEINLINE void                SetData(V nID, D data)
    {
        m_DataLst.Add(nID, data);
    }

public:

    virtual FORCEINLINE int32               GetDataNum()
    {
        return m_DataLst.Num();
    }

    virtual FORCEINLINE const D*            GetData(V nID)
    {
        return m_DataLst.Find(nID);
    }

    virtual FORCEINLINE const D             GetDataRef(V nID)
    {
        return m_DataLst.FindRef(nID);
    }

    virtual FORCEINLINE FDataMap*           GetData()
    {
        return &m_DataLst;
    }

public:
    FORCEINLINE bool                        IsLoaded()
    {
        return m_bIsLoaded;
    }

    FORCEINLINE BaseUnitManager*            GetTableManager()
    {
        return &m_TableManager;
    }

    FORCEINLINE bool                        IsVaild(V ID)
    {
        return m_DataLst.Contains(ID);
    }

private:
    T *                                     m_pThis;
    FDataMap                                m_DataLst;
    FString                                 m_FileName;
    bool                                    m_bIsLoaded;

    BaseUnitManager                         m_TableManager;
};


//////////////////////////////////////////////////////////////////
/************************************************************************
* Desc 	: ini配置文件读取模板类
* Param	: FileName：不带路径的文件名，默认地址 Content\Configs目录下
* Return	: 
* Author	: WangPeng
* Time		: [2/22/2019 wp]
************************************************************************/

template<class T, typename D>
class TABLEMODULE_API SIniBaseTable : public SSingleton<T>
{
    GENERATED_SUPER(SIniBaseTable)

public:
    SIniBaseTable(T* pThis) :m_pThis(pThis), m_bIsLoaded(false) { m_FileName.Empty(); }
    SIniBaseTable(T* pThis, FString FileName) :m_pThis(pThis), m_FileName(FileName), m_bIsLoaded(false) {  }

	typedef D FData;
    virtual ~SIniBaseTable()
    {
        m_FileName.Empty();
        m_pThis = NULL;
        m_bIsLoaded = false;
    }

public:
    virtual bool                            LoadTable()
    {
        if (!m_pThis || m_FileName.IsEmpty())
            return false;

        m_bIsLoaded = false;

        if (!m_TableManager.LoadIni(m_FileName))
        {
            Logs(TEXT("load %s failed!"), true, 20.f, FColor::Red, *m_FileName);
            return false;
        }

        if (!((T*)m_pThis)->ReadTable(&m_TableManager))
        {
            Logs(TEXT("ReadTable %s failed!"), true, 20.f, FColor::Red, *m_FileName);
            m_TableManager.UnLoadIni();
            return false;
        }

        m_TableManager.UnLoadIni();

        m_bIsLoaded = true;
        return true;
    }

    virtual bool                            LoadTable(FString FileName)
    {
        if (!m_pThis || FileName.IsEmpty())
            return false;

        m_bIsLoaded = false;

        if (!m_TableManager.LoadIni(FileName))
        {
            Logs(TEXT("load %s failed!"), true, 20.f, FColor::Red, *FileName);
            return false;
        }

        if (!((T*)m_pThis)->ReadTable(&m_TableManager))
        {
            Logs(TEXT("ReadTable %s failed!"), true, 20.f, FColor::Red, *FileName);
            m_TableManager.UnLoadIni();
            return false;
        }

        m_TableManager.UnLoadIni();
        m_FileName = FileName;
        m_bIsLoaded = true;
        return true;
    }


    virtual bool                            ReadTable(BaseUnitManager* pTableManager) = 0;

protected:
    virtual FORCEINLINE void                SetData(FData data)
    {
        m_Data = data;
    }

public:
    FORCEINLINE bool                        IsLoaded()
    {
        return m_bIsLoaded;
    }

    virtual FORCEINLINE const FData*            GetData()
    {
        return &m_Data;
    }

    virtual FORCEINLINE const FData             GetDataRef()
    {
        return m_Data;
    }

	virtual FORCEINLINE FString				GetFileName()
	{
		return m_FileName;
	}

private:
	T*										m_pThis;
	FString                                 m_FileName;
    bool                                    m_bIsLoaded;
	FData                                   m_Data;
    BaseUnitManager                         m_TableManager;

};

