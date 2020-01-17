// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseSystem/SingletonBase.h"
#include "SPregameManager.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
namespace EStreamLevelState
{
	enum Type
	{
		Launcher,                     //
		CreateRole                    //
	};
}

UCLASS()
class OVERLORD_API UPregameManager : public UObject , public FSingletonBase<UPregameManager>
{
	GENERATED_BODY()

private:
	int32		m_Status;

public:
	virtual void			BeginDestroy() override;

	void					ShowLauncher();
	void					ShowLogin();
	void					ShowCreateRole();
	void					ShowSelectRole();

	void					Tick(float fDeltaTime);

private:
	/*********************************函数声明********************************/
	// Desc 	: StreamLevel 卸载回调函数
	// Param	: EStreamLevelState 枚举值
	// Return	: 
	// Author	: WangPeng
	// Time		: 2019.5.24
	/************************************************************************/
	UFUNCTION()
		void				UnloadSLFinished(int32 LinkID);

	UFUNCTION()
		void				LoadSLFinished(int32 LinkID);
	
};
