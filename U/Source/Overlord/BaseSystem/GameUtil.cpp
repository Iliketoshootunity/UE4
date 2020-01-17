// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseSystem/GameUtil.h"
#include "Engine.h"
#include "BaseDefine.h"
#include "SkeletalMeshMerge.h"
#include "BasicFunction.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/Skeleton.h"

FStreamableManager* UGameUtil::m_pAssetLoader = NULL;

UGameUtil::UGameUtil()
{
    m_pAssetLoader = new FStreamableManager();
}

UGameUtil::~UGameUtil()
{
    if (m_pAssetLoader)
        delete m_pAssetLoader;

    m_pAssetLoader = NULL;
}

UObject* UGameUtil::SyncLoadAsset(FString Filepath)
{
    if (Filepath.IsEmpty() || !m_pAssetLoader)
        return nullptr;

    FStringAssetReference testAssetRef = Filepath;
    UObject* pObject = testAssetRef.ResolveObject();

    if (pObject == nullptr)
    {
        pObject = m_pAssetLoader->LoadSynchronous(testAssetRef, true);
    }

    if (pObject == nullptr)
    {
        FBasicFunction::Logs(TEXT("Don't have this Asset:[ %s ]"), *Filepath);
    }

    return pObject;
}

bool UGameUtil::UnLoadAsset(FString Filepath)
{
    if (Filepath.IsEmpty() || !m_pAssetLoader)
        return false;

    FStringAssetReference testAssetRef = Filepath;

    m_pAssetLoader->Unload(testAssetRef);

    return true;
}

bool UGameUtil::DestroyAsset(UObject* pObject)
{
    if (!pObject || !pObject->IsValidLowLevel())
        return false;

#if !WITH_EDITOR
    pObject->ConditionalBeginDestroy();
    pObject = nullptr;
#endif
	//pObject->MarkPendingKill();

    //立即调用gc，回收清理，如果不调用，系统每60s清理一次。这时才会调用析构函数
    //BaseEngine.ini gc.TimeBetweenPurgingPendingKillObjects=60 调整时间间隔
    if (GEngine != NULL)
    {
        GEngine->ForceGarbageCollection(true);
    }
   /* else if (GetGameInstance() && GetGameInstance()->GetEngine())
    {
        GetGameInstance()->GetEngine()->ForceGarbageCollection(true);
    }*/

    return true;
}

bool UGameUtil::DestroyAsset(AActor* pObject)
{
    if (!pObject || !pObject->IsValidLowLevel())
        return false;

    pObject->Destroy();
    pObject->ConditionalBeginDestroy();
    pObject = nullptr;

    //立即调用gc，回收清理，如果不调用，系统每60s清理一次。这时才会调用析构函数
    //BaseEngine.ini gc.TimeBetweenPurgingPendingKillObjects=60 调整时间间隔
    if (GEngine != NULL)
    {
        GEngine->ForceGarbageCollection(true);
    }
    /*else if (GetGameInstance() && GetGameInstance()->GetEngine())
    {
        GetGameInstance()->GetEngine()->ForceGarbageCollection(true);
    }*/

    return true;
}

bool UGameUtil::DestroyObject(UObject* pObject)
{
	return DestroyAsset(pObject);
}

bool UGameUtil::DestroyActor(AActor* pActor)
{
	return DestroyAsset(pActor);
}

void UGameUtil::FreeMemory()
{
    if (GEngine != NULL)
        GEngine->ForceGarbageCollection(true);
}

USkeletalMesh* UGameUtil::SkeletalMeshMerge(TArray<USkeletalMesh*> SrcMeshList, USkeleton* pSkeleton)
{
    if (SrcMeshList.Num() == 0) return NULL;

    TArray<USkeletalMesh*>& SourceMeshList = SrcMeshList;
    USkeleton* _NewSkeleton = pSkeleton != NULL ? pSkeleton : SrcMeshList[0]->Skeleton;
    auto CompositeMesh = NewObject<USkeletalMesh>(GetTransientPackage(), NAME_None, RF_Transient);
    TArray<FSkelMeshMergeSectionMapping> InForceSectionMapping;
    FSkeletalMeshMerge MeshMergeUtil(CompositeMesh, SourceMeshList, InForceSectionMapping, 0);
    if (!MeshMergeUtil.DoMerge())
    {
		FBasicFunction::Logs(TEXT("DoMerge Error: Merge Mesh Test Failed"));
        return NULL;
    }
    CompositeMesh->Skeleton = _NewSkeleton;    //重新绑定骨骼
    return CompositeMesh;
}


USkeletalMesh* UGameUtil::SkeletalMeshMerge(TArray<FString> SrcMeshFileList, FString SkeletonName)
{
    if (SrcMeshFileList.Num() == 0) return NULL;

    TArray<USkeletalMesh*> SrcMeshList;
    for (auto SkeletalMashName : SrcMeshFileList)
    {
        USkeletalMesh* pSkelMesh = LoadClass<USkeletalMesh>(SkeletalMashName);
        if (!pSkelMesh) continue;

        SrcMeshList.Add(pSkelMesh);
    }

    if (SrcMeshList.Num() <= 0) return NULL;

    auto CompositeMesh = NewObject<USkeletalMesh>(GetTransientPackage(), NAME_None, RF_Transient);
    TArray<FSkelMeshMergeSectionMapping> InForceSectionMapping;
    FSkeletalMeshMerge MeshMergeUtil(CompositeMesh, SrcMeshList, InForceSectionMapping, 0);
    if (!MeshMergeUtil.DoMerge())
    {
		FBasicFunction::Logs(TEXT("DoMerge Error: Merge Mesh Test Failed"));
        return NULL;
    }
    CompositeMesh->Skeleton = SkeletonName.IsEmpty() ? SrcMeshList[0]->Skeleton : LoadClass<USkeleton>(SkeletonName);   //重新绑定骨骼
    return CompositeMesh;
}

bool UGameUtil::SkeletalMeshMerge(TArray<USkeletalMesh*> SrcMeshList, USkeletalMeshComponent* SkeletalComp)
{
    if (SrcMeshList.Num() == 0 || SkeletalComp == NULL) return false;

    USkeletalMesh* pSkeletalMesh = UGameUtil::SkeletalMeshMerge(SrcMeshList, SkeletalComp->SkeletalMesh ? SkeletalComp->SkeletalMesh->Skeleton : NULL);
    if (!pSkeletalMesh) return false;

    SkeletalComp->SetSkeletalMesh(pSkeletalMesh);
    return true;
}


float UGameUtil::GetLoadingPercentage(const FName& PackageName)
{
    return GetAsyncLoadPercentage(PackageName);
}

