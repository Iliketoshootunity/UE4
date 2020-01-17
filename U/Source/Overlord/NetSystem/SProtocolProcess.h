// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IMsgProtocol.h"
#include "Protoc/enum.pb.h"

/**
 *
 */
class OVERLORD_API SProtocolProcess : public IMsgProtocol
{
	typedef void(SProtocolProcess::* FuncPtr)(uint8* pMsg, int32 length);
public:

	SProtocolProcess();
	virtual ~SProtocolProcess();

	virtual bool    ProcessMsg(int32 usProtocolType, void* pMsgData, int32 length) override;

private:
	//void (SProtocolProcess::* ProcessFunc[s2c_PROTOCOL_ARRAYSIZE])(uint8* pMsg, int32 length);

	TMap<int32, FuncPtr> m_FuncMap;


	int64 TestMyID;
	int32 TestMoveIndex;

private:
	void            s2cPing(uint8* pMsg, int32 length);
	void            s2cLogin(uint8* pMsg, int32 length);                          //服务器验证登录，错误时返回此协议
	void            s2cSyncworld(uint8* pMsg, int32 length);
	void            s2cRoleLsit(uint8* pMsg, int32 length);                       //服务器验证登录，正确时返回角色列表
	void			s2cSynclocalplayer(uint8* pMsg, int32 length);
	void            s2cMinCharacter(uint8* pMsg, int32 length);
	void            s2cCharacter(uint8* pMsg, int32 length);
	void			s2cDisconectInfo(uint8* pMsg, int32 length);

	void			s2cComboNodeMessage(uint8* pMsg, int32 length);
	void			s2cComboEndMessage(uint8* pMsg, int32 length);
	void			ResRoleInfo(uint8* pMsg, int32 length);

	void			ResUpdateViewMessage(uint8* pMsg, int32 length);
	void			ResMonsterEnterViewMessage(uint8* pMsg, int32 length);
	void			ResPlayerEnterViewMessage(uint8* pMsg, int32 length);
	void			ResMapObjectExitViewMessage(uint8* pMsg, int32 length);

	void			ResMoveMessage(uint8* pMsg, int32 length);

	void			ResFightResult(uint8* pMsg, int32 length);

	void			ResHeartMessage(uint8* pMsg, int32 length);
};

extern OVERLORD_API SProtocolProcess g_ProtocolProcess;
