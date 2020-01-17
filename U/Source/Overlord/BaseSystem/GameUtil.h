// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/StreamableManager.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"
#include "UserWidget.h"
#include "GameUtil.generated.h"

/**
 * 
 */
class USkeletalMesh;
class USkeleton;

UCLASS()
class OVERLORD_API UGameUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    UGameUtil();
    virtual ~UGameUtil();

private:
    static FStreamableManager*              m_pAssetLoader;

public:

    template <class TGameInstance = UGameInstance>
    static TGameInstance* GetGameInstance()
    {
        if (GEngine && GEngine->GetWorld())
        {
            return Cast<TGameInstance>(GEngine->GetWorld()->GetGameInstance());
        }

        return nullptr;
    }

    //Load  UMG��umap��UTexture2D
    UFUNCTION(BlueprintPure, Category = GameUtil)
    static UObject* SyncLoadAsset(FString Filepath);

    /************************************************************************
    * Desc 	: ж���Ѿ����ص���Դ
    * Param	: ��Դ��+·��
    * Return	:
    * Author	: WangPeng
    * Time		: [2/23/2019 wp]
    ************************************************************************/
    UFUNCTION(BlueprintPure, Category = GameUtil)
    static bool UnLoadAsset(FString Filepath);

    /*���ò���ʱ,������Դ���󣬵����ͷ�*/
    UFUNCTION(BlueprintPure, meta = (DisplayName = "DestroyAsset"), Category = GameUtil)
    static bool DestroyAsset(UObject* pObject);

    /*���ò���ʱ,������Դ���󣬵����ͷ�*/
    static bool DestroyAsset(AActor* pObject);

	static bool DestroyObject(UObject* pObject);

	static bool DestroyActor(AActor* pActor);

    /*�ͷ���Ϸ�ڴ棬���տ����ڴ�*/
    static void FreeMemory();

public:
    /************************************************************************
    * Desc 	: ����Widget����
    * Param	:
    * Return	:
    * Author	: WangPeng
    * Time		: [2/23/2019 wp]
    ************************************************************************/
    template<typename T>
    static T* LoadWidget(FString FilePath, UGameInstance* pGameInstance = NULL)
    {
        UObject* pObject = SyncLoadAsset(FilePath);

        if (pObject == nullptr || GEngine == nullptr)
            return nullptr;

        UGameInstance* _pGameInstance = pGameInstance == NULL ? GEngine->GetWorld()->GetGameInstance() : pGameInstance;

        return CreateWidget<T>(_pGameInstance, Cast<UClass>(pObject));
    }

    template<typename T = UObject>
    static T*  LoadClass(FString FilePath)
    {
        UObject* LoadObj = SyncLoadAsset(FilePath);
        return LoadObj == nullptr ? NULL : Cast<T>(LoadObj);
    }

    //�ϲ�ģ��
    static USkeletalMesh*           SkeletalMeshMerge(TArray<USkeletalMesh*> SrcMeshList, USkeleton* pSkeleton = NULL);

    static USkeletalMesh*           SkeletalMeshMerge(TArray<FString> SrcMeshFileList, FString SkeletonName);

    UFUNCTION(BlueprintCallable, Category = GameUtil)
    static bool                     SkeletalMeshMerge(TArray<USkeletalMesh*> SrcMeshList, USkeletalMeshComponent* SkeletalComp);

    //
    UFUNCTION(BlueprintCallable, Category = GameUtil)
    static float GetLoadingPercentage(const FName& PackageName);

};
