// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SUIDefine.h"
#include "BaseSystem/SingletonBase.h"
#include "SLoadingManager.generated.h"

/**
 * 
 */
UCLASS()
class OVERLORD_API ULoadingManager : public UObject , public FSingletonBase<ULoadingManager>
{
	GENERATED_BODY()

public:
	ULoadingManager();

public:
	void				Init();

	void				ShowLoading(ELoadingMark::Type eMark, int32 nLoadingScreenID);

	void				StopLoading();
	/************************************************************************
	* Desc 	: Loading 播放结束回调
	* Param	: 当前结束的此次播放回调返回标记
	* Return	:
	* Author	: WangPeng
	* Time		: [3/22/2019 wp]
	************************************************************************/
	void				OnLoadingFinished(const int32 nMark);
	
};
