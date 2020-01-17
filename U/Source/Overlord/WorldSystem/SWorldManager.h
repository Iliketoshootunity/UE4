// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseSystem/SingletonBase.h"
#include "SWorld.h"
#include "Protoc/map.pb.h"
#include "NpcsTable.h"
#include "NpcSystem/Other/CSCharacterDefine.h"
#include "NpcSystem/GameCharacter/CSGameCharacter.h"
#include "NpcSystem/Character/CSCharacter.h"
#include "SWorldManager.generated.h"

/**
 *
 */

#define  CREATECHARACTERCOUNTEVERYFRAME 1

class FMessage;

UCLASS()
class OVERLORD_API UWorldManager : public UObject, public FSingletonBase<UWorldManager>
{
	GENERATED_BODY()

private:
	//前端只可能存在一个map
	FWorld                                             m_CurrentMap;
	TMap<int32, TArray<UCSGameCharacter*>>	  CharacterMapList;
	UPROPERTY()
		TMap<int64, UCSGameCharacter*>	  CharacterMap;
	UPROPERTY()
		UCSGameCharacter* MainPlayer;
	UPROPERTY()
		TArray<UCSGameCharacter*> WaitCreateCharacterList;

protected:
	bool bLoadMapFinshed;

public:
	UCSGameCharacter* GetMainPlayer()
	{
		return MainPlayer;
	}
public:

	void    Init();
	void    Tick(float fDeltaTime);
	void    Shutdown();

	int64  TestMyID;
protected:
	void	BindNetMessage();
	void	UnBindNetMessage();
	void	OnResUpdateViewMessage(FMessage* Message);
	void	OnResPlayerEnterViewMessage(FMessage* Message);
	void	OnResMonsterEnterViewMessage(FMessage* Message);
	void	OnResMapObjectExitViewMessageMessage(FMessage* Message);
	void	OnResComboNodeMessage(FMessage* Message);
	void	OnResComboEndNodeMessage(FMessage* Message);
	void	OnResFightResultMessage(FMessage* Message);
	void	OnResMoveMessage(FMessage* Message);
	void	OnResDisconectInfo(FMessage* Message);
	template<typename T = UCSGameCharacter>
	void	ProcessCharacterEnterView(int64 Id, void* Info, ECharaterType CharacterType)
	{
		if (Id == 0)
		{
			return;
		}
		//移除
		//RemoveCharacter(Id);
		//创建
		T* Character = CreateCharacter<T>();
		if (Character)
		{
			//初始化，Info在初始化中创建
			Character->Init(Info);
			//添加
			AddCharacter(CharacterType, Character);
			//等待创建实例
			WaitCreateCharacterList.Add(Character);
		}
		if (CharacterType == ECharaterType::MainPlayer)
		{
			MainPlayer = Character;
		}
	}

public:
	/*创建角色逻辑主体*/
	template<typename T = UCSGameCharacter>
	T * CreateCharacter()
	{
		T* t = NewObject<T>(this);
		return t;
	}
	/*创建角色模型主体*/
	ACSCharacter* CreateCharacterUnit(UCSGameCharacter* Character);
	/*添加角色*/
	void	AddCharacter(ECharaterType CharacterType, UCSGameCharacter* BaseCharacter);
	/*移除角色*/
	void	RemoveCharacter(int64 Id);
	/*遍历循环创建实体*/
	void	UpdateAndCreateWaitList();
	/*每帧发送消息：移动*/
	void    TickMessage(float DeltaTime);
public:
	UCSGameCharacter* GetCharacter(int64 Id);

public:
	bool   LoadMap(int32 MapID, bool bShowLoading = true);
	bool   UnloadMap();
private:


	UFUNCTION()
		//加载关卡完成后调用
	void                                                OnLoadLevelFinished(int32 LinkID);

public:
	FORCEINLINE  FWorld* GetSence()
	{
		return &m_CurrentMap;
	}

private:
	void												ShowLoadingScreen();
	void												HideLoadingScreen();

};

