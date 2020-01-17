// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGameDefine.h"


//����ID�Ľṹ��
typedef struct tagItemID
{
    int32 nGeneral;
    int32 nDetail;
    int32 nParticular;
    int32 nPart;
    int32 nCount;
}FItemID;

//���ߵĻ�������
typedef struct tagItemBaseData : public FItemID
{
    int32 nCount;
}FItemBaseData;

// Struct
typedef struct tagItemBasicProp
{
	FString szName;			// ��������
	int ID;					// ID Key

	int32 StringID;			// ������ StringID
    FItemID ItemID;         // ����ID
    int32 Type;             // ��������
	int32 MeshPartIndex;	// ��װ����
	FString DropIcon;		// ����ͼ��
	FString	ItemIcon;		// ����ͼ��
	int32 PhysicAttack;		// ������
	int32 AttackSpeed;		// �����ٶ�
    int32 NormalSkillID;	// ��ͨ��������ID

}ItemBasicProp;



//ItemList ��Item��Ϣ
typedef struct tagItemPos
{
    int32		nRoomPlace;     // ItemPosition  ��λ��
    int32		nX;             //Room �����ڵľ������width
    int32		nY;             //Room �����ڵľ������heidth
} ItemPos;

//ItemList ��Item��Ϣ
typedef struct tagPlayerItem
{
    uint64		nItemId;        //����ID
    ItemPos		ItemPosInfo;     // ItemPos Xin��
} PlayerItem;

