// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGameDefine.h"


//道具ID的结构体
typedef struct tagItemID
{
    int32 nGeneral;
    int32 nDetail;
    int32 nParticular;
    int32 nPart;
    int32 nCount;
}FItemID;

//道具的基本数据
typedef struct tagItemBaseData : public FItemID
{
    int32 nCount;
}FItemBaseData;

// Struct
typedef struct tagItemBasicProp
{
	FString szName;			// 道具名字
	int ID;					// ID Key

	int32 StringID;			// 武器名 StringID
    FItemID ItemID;         // 道具ID
    int32 Type;             // 武器类型
	int32 MeshPartIndex;	// 换装部件
	FString DropIcon;		// 掉落图标
	FString	ItemIcon;		// 道具图标
	int32 PhysicAttack;		// 物理攻击
	int32 AttackSpeed;		// 攻击速度
    int32 NormalSkillID;	// 普通攻击技能ID

}ItemBasicProp;



//ItemList 中Item信息
typedef struct tagItemPos
{
    int32		nRoomPlace;     // ItemPosition  的位置
    int32		nX;             //Room 容器内的具体格子width
    int32		nY;             //Room 容器内的具体格子heidth
} ItemPos;

//ItemList 中Item信息
typedef struct tagPlayerItem
{
    uint64		nItemId;        //道具ID
    ItemPos		ItemPosInfo;     // ItemPos Xin西
} PlayerItem;

