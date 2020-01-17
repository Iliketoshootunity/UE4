// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoftObjectPath.h"


/************************************************************************
* Desc 	: 资源封装类，目前只支持但资源加载。 
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
    * Desc 	: 只有异步加载时（AssetAsyncLoad），事件才会起效
    * Param	: AssetName : 资源名；InFinishDelegate : 异步加载完成事件；InProgressDelegate : 异步加载进度事件;
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
    * Desc 	: 调用此函数注意，是否已经加载完成。加载Level的话一定要加载完成后，再调用此函数
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
    * Desc 	: 异步加载资源
    * Param	: InDelegate 回调函数 ；FStreamableDelegate::CreateSP(this, &FAssetRef::AsyncCallBack)
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/14/2019 wp]
    ************************************************************************/
    void                                AssetAsyncLoad();

    void                                AsyncCallBack();

    UObject*                            GetAsyncAssetObject();

    /************************************************************************
    * Desc 	: 同步加载
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/14/2019 wp]
    ************************************************************************/
    UObject*                            AssetSyncLoad();

    void                                Unload();

    /************************************************************************
    * Desc 	: 已经加载过的资源进行处理
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/15/2019 wp]
    ************************************************************************/
    static UObject*                     GetObject(FString AssetName);
    static void                         Unload(FString AssetName);

private:
    /************************************************************************
    * Desc 	: 只有异步加载，才调用Tick函数，需要获取加载进度
    * Param	: 
    * Return	: 
    * Author	: WangPeng
    * Time		: [3/15/2019 wp]
    ************************************************************************/
    void                                BeginTick();
    void                                StopTick();
};
