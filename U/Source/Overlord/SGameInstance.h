// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGameInstance.generated.h"

/**
 * 
 */
class ITcpNetworkAgent;
class FMessage;
class ACSPlayer;
class FMessageDispatcher;

UCLASS()
class OVERLORD_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USGameInstance();
	virtual ~USGameInstance();

public:
	virtual void                        Init() override;
	virtual void                        Shutdown() override;
	virtual void                        StartGameInstance() override;
#if WITH_EDITOR
	virtual FGameInstancePIEResult      InitializeForPlayInEditor(int32 PIEInstanceIndex, const FGameInstancePIEParameters& Params) override;
	virtual FGameInstancePIEResult      StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params) override;
#endif

public:
	void								QuitGame();
private:
	/************************************************************************
	* Desc 	: 初始化网络模块
	* Param	:
	* Return	:
	* Author	: WangPeng
	* Time		: [2/22/2019 wp]
	************************************************************************/
	void                                InitNetwork();

	bool                                Tick(float fDeltaTime);

public:
	/************************************************************************
	* Desc 	: 获取网络层句柄
	* Param	:
	* Return	: IClientNetConnectAgent类型
	* Author	: WangPeng
	* Time		: [2/27/2019 wp]
	************************************************************************/
	FORCEINLINE ITcpNetworkAgent*		GetNet()
	{
		return m_pNetConnectAgent;
	}

	bool								ConnectServer(FString IP, int32 nPort, bool IsTcp = true);

	void								DisConnectServer();

	bool								SendMessage(int32 protocolID, void* pMessage, int16 mark = 0);
	
	void								SendPlayerWalkMessage(float DeltaTime);

	//测试
	UFUNCTION(BlueprintCallable)
	void								SendOtherCharacterInfoMessage();

	void								SetPlayerCharacter(ACSPlayer* Player);
	UPROPERTY()
	ACSPlayer*							PlayerCharacter;

private:
	FDelegateHandle                     m_TickDelegateHandle;
	ITcpNetworkAgent*				    m_pNetConnectAgent;

};

extern OVERLORD_API USGameInstance*		g_pGameInstance;
