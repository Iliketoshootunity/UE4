// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoftObjectPath.h"


/************************************************************************
* Desc 	: ��Դ��װ�࣬Ŀǰֻ֧�ֵ���Դ���ء� 
* Author	: WangPeng
* Time		: [3/14/2019 wp]
************************************************************************/

DECLARE_DELEGATE_OneParam(FAssetRefLoadFinishDelegate, FName);
DECLARE_DELEGATE_OneParam(FAssetRefLoadProgressDelegate, float);

class OVERLORD_API FAssetRef //: public TSharedFromThis<FAssetRef>
{
private:
    FStringAssetReference               m_AssetRef;
    FAssetRefLoadFinishDelegate         m_LoadFinishDelegate;
    FAssetRefLoadProgressDelegate       m_LoadProgressDelegate;
    FDelegateHandle                     m_TickDelegateHandle;

public:
    FAssetRef();
	~FAssetRef();

    /************************************************************************
    * Desc 	: ֻ���첽����ʱ��AssetAsyncLoad�����¼��Ż���Ч
    * Param	: AssetName : ��Դ����InFinishDelegate : �첽��������¼���InProgressDelegate : �첽���ؽ����¼�;
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/15/2019 wp]
    ************************************************************************/
    bool                                Init(
        FString AssetName, 
        FAssetRefLoadFinishDelegate InFinishDelegate = FAssetRefLoadFinishDelegate(),
        FAssetRefLoadProgressDelegate InProgressDelegate = FAssetRefLoadProgressDelegate()
    );

    bool                                Tick(float fDeltaTime);

    /************************************************************************
    * Desc 	: ���ô˺���ע�⣬�Ƿ��Ѿ�������ɡ�����Level�Ļ�һ��Ҫ������ɺ��ٵ��ô˺���
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/15/2019 wp]
    ************************************************************************/
    void                                Reset();

    FORCEINLINE bool                    IsValid()
    {
        return m_AssetRef.IsValid();
    }

    FORCEINLINE bool                    IsLoaded()
    {
        return IsValid() ? m_AssetRef.ResolveObject() != NULL : false;
    }

    FORCEINLINE void                    RegistLoadFinishEvent(FAssetRefLoadFinishDelegate InFinishDelegate)
    {
        m_LoadFinishDelegate = InFinishDelegate;
    }

    FORCEINLINE void                    RegistLoadProgressEvent(FAssetRefLoadProgressDelegate InProgressDelegate)
    {
        m_LoadProgressDelegate = InProgressDelegate;
    }

    /************************************************************************
    * Desc 	: �첽������Դ
    * Param	: InDelegate �ص����� ��FStreamableDelegate::CreateSP(this, &FAssetRef::AsyncCallBack)
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/14/2019 wp]
    ************************************************************************/
    void                                AssetAsyncLoad();

    void                                AsyncCallBack();

    UObject*                            GetAsyncAssetObject();

    /************************************************************************
    * Desc 	: ͬ������
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/14/2019 wp]
    ************************************************************************/
    UObject*                            AssetSyncLoad();

    void                                Unload();

    /************************************************************************
    * Desc 	: �Ѿ����ع�����Դ���д���
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/15/2019 wp]
    ************************************************************************/
    static UObject*                     GetObject(FString AssetName);
    static void                         Unload(FString AssetName);

private:
    /************************************************************************
    * Desc 	: ֻ���첽���أ��ŵ���Tick��������Ҫ��ȡ���ؽ���
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/15/2019 wp]
    ************************************************************************/
    void                                BeginTick();
    void                                StopTick();
};
