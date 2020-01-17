// Fill out your copyright notice in the Description page of Project Settings.

#include "SAssetRef.h"
#include "Engine/AssetManager.h"
#include "Ticker.h"

FAssetRef::FAssetRef()
{
	
}

FAssetRef::~FAssetRef()
{
    Reset();
	StopTick();
}

bool FAssetRef::Init(FString AssetName, FAssetRefLoadFinishDelegate InFinishDelegate, FAssetRefLoadProgressDelegate InProgressDelegate)
{
    if (AssetName.IsEmpty()) return false;
    if (IsValid()) return false;

    m_AssetRef = AssetName;
	if(!m_LoadFinishDelegate.IsBound())
		m_LoadFinishDelegate = InFinishDelegate;

	if(!m_LoadProgressDelegate.IsBound())
		m_LoadProgressDelegate = InProgressDelegate;
 
    return true;
}

bool FAssetRef::Tick(float fDeltaTime)
{
    if (!IsValid()) return false;

    m_LoadProgressDelegate.ExecuteIfBound(GetAsyncLoadPercentage(m_AssetRef.GetAssetPathName()));

    return true;
}

void FAssetRef::Reset()
{
    m_AssetRef.Reset();

}

void FAssetRef::AssetAsyncLoad()
{
	if (IsLoaded())
	{
		AsyncCallBack();
		return;
	}
 
	BeginTick();

    UAssetManager::GetStreamableManager().RequestAsyncLoad(m_AssetRef, FStreamableDelegate::CreateRaw(this, &FAssetRef::AsyncCallBack));
}

void FAssetRef::AsyncCallBack()
{
    if (!IsValid()) return;

    m_LoadFinishDelegate.ExecuteIfBound(m_AssetRef.GetAssetPathName());

    StopTick();

	m_LoadProgressDelegate.Unbind();

	m_LoadFinishDelegate.Unbind();
}

UObject* FAssetRef::GetAsyncAssetObject()
{
    if (!IsValid()) return NULL;
    return m_AssetRef.ResolveObject();
}

UObject* FAssetRef::AssetSyncLoad()
{
    if (!IsValid()) return NULL;

    UObject* pObject = m_AssetRef.ResolveObject();

    if (!pObject)
    {
        pObject = UAssetManager::GetStreamableManager().LoadSynchronous(m_AssetRef, true);
    }

    return pObject;
}

void FAssetRef::Unload()
{
    if (!IsLoaded()) return;

    UAssetManager::GetStreamableManager().Unload(m_AssetRef);
}


UObject* FAssetRef::GetObject(FString AssetName)
{
    FStringAssetReference AssetRef = AssetName;

    return AssetRef.ResolveObject();
}

void FAssetRef::Unload(FString AssetName)
{
    FStringAssetReference AssetRef = AssetName;
    UAssetManager::GetStreamableManager().Unload(AssetRef);
}


void FAssetRef::BeginTick()
{
    FTickerDelegate TickDelegate = FTickerDelegate::CreateRaw(this, &FAssetRef::Tick);
    m_TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
}

void FAssetRef::StopTick()
{
    if (m_TickDelegateHandle.IsValid())
        FTicker::GetCoreTicker().RemoveTicker(m_TickDelegateHandle);
}
