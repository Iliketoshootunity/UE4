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
	//ǰ��ֻ���ܴ���һ��map
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
		//�Ƴ�
		//RemoveCharacter(Id);
		//����
		T* Character = CreateCharacter<T>();
		if (Character)
		{
			//��ʼ����Info�ڳ�ʼ���д���
			Character->Init(Info);
			//���
			AddCharacter(CharacterType, Character);
			//�ȴ�����ʵ��
			WaitCreateCharacterList.Add(Character);
		}
		if (CharacterType == ECharaterType::MainPlayer)
		{
			MainPlayer = Character;
		}
	}

public:
	/*������ɫ�߼�����*/
	template<typename T = UCSGameCharacter>
	T * CreateCharacter()
	{
		T* t = NewObject<T>(this);
		return t;
	}
	/*������ɫģ������*/
	ACSCharacter* CreateCharacterUnit(UCSGameCharacter* Character);
	/*��ӽ�ɫ*/
	void	AddCharacter(ECharaterType CharacterType, UCSGameCharacter* BaseCharacter);
	/*�Ƴ���ɫ*/
	void	RemoveCharacter(int64 Id);
	/*����ѭ������ʵ��*/
	void	UpdateAndCreateWaitList();
	/*ÿ֡������Ϣ���ƶ�*/
	void    TickMessage(float DeltaTime);
public:
	UCSGameCharacter* GetCharacter(int64 Id);

public:
	bool   LoadMap(int32 MapID, bool bShowLoading = true);
	bool   UnloadMap();
private:


	UFUNCTION()
		//���عؿ���ɺ����
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

