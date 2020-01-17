// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// ... add public include paths required here ...

#include "SGameDefine.generated.h"



// ... add public Defines required here ...
#define MAX_ACCOUNT		32
#define MAX_PASSWORD	64
#define MAX_NAME_LEN	32


//网络传送整形坐标
USTRUCT(immutable, noexport, BlueprintType)
struct SHARED_API FSVector
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vector, SaveGame)
        int32 X;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vector, SaveGame)
        int32 Y;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vector, SaveGame)
        int32 Z;

public:
    FORCEINLINE FSVector()
    {
        X = 0;
        Y = 0;
        Z = 0;
    }

    FORCEINLINE FSVector(int32 InX, int32 InY, int32 InZ)
    {
        X = InX;
        Y = InY;
        Z = InZ;
    }

    FORCEINLINE FVector ToFVector()
    {
        return FVector(X, Y, Z);
    }
};





UENUM(BlueprintType)
namespace EAnimState
{
	enum Type
	{
		do_None = 0,
		do_Blink,
		do_Dead,
		do_Hit,
		do_HiHit,
		do_Skill,
	};
}

// Enum
//道具种类 ItemGener
UENUM(BlueprintType)
namespace ItemGener
{
    enum Type
    {
        item_invalid = -1,
        item_equip,
        item_medicine,
        item_number
    };
}

//道具具体类别 ItemDetail
UENUM(BlueprintType)
namespace ItemDetail
{
    enum Type
    {
        item_invalid = -1,
        item_helm,                  //头盔0
        item_armor,                 //盔甲1
        item_shoulder,              //护肩2
        item_belt,                  //腰带3
		item_leg,					//腿4
		item_weapon,				//武器5
		item_ride,					//坐骑6
        item_number,
    };
}

//道具详细类别 ItemIndex
//道具详细类别为道具index [0 ... max ]

//道具最小类别 ItemMinCategory
/*weapon表 分类（剑：1，拳套：2，琴：4，长柄：8 ）
(包含时配置说明 其他配置表 配 1 代表剑，2（拳套），3（剑+拳套），4（琴），5（剑+琴），6（琴+拳套）
，7（琴+剑+拳套），8（长柄），9（长柄+剑），10（长柄+拳套），11（长柄+剑+拳套）,12(长柄+琴)，13（长柄+剑+琴），14（长柄+拳套+琴），15（长柄+剑+拳套+琴））
用法：（仅能对武器的限制）WeaponSkill & nMinCategory （最小类别）
*/
//其他表最小类别功能预留


//装备最小类别,关联角色装备位
UENUM(BlueprintType)
namespace EquipPart
{
    enum Type
    {
        part_invalid = -1,
        part_main_weapon,	// 装备武器1 Weapon 配置part [0：都不能安放，1：part_main_weapon，2：part_weapon1，3：（part_main_weapon，part_weapon2），4：part_weapon2]
        part_weapon1,	// 装备武器2    [5：(part_main_weapon,part_weapon2), 6:(part_weapon1,part_weapon2), 7:(part_main_weapon,part_weapon1,part_weapon2)]
        part_weapon2,	// 装备武器3
        part_helm,		// 装备头盔 4
        part_num		// 装备部位数目
    };
}


/************************************************************************
* Desc 	: BodyPart.txt对应的部件模型资源（Item-Mesh res）
* Param	: 
* Return	: 
* Author	: WangPeng
* Time		: [2/28/2019 wp]
************************************************************************/
UENUM(BlueprintType)
namespace BODY_PART
{
    enum Type
    {
		BODY_PART_INVALID = -1,
        BODY_PART_HAIR,                     //头发、头盔
		BODY_PART_FACE,                     //脸
		BODY_PART_SHOULDER,                 //肩
        BODY_PART_BODY,                     //身体 
        BODY_PART_BELT,                     //腰
        BODY_PART_LEG,                      //腿
		BODY_PART_LHAND_WEAPON,             //左手武器
		BODY_PART_RHAND_WEAPON,				//右手武器
        BODY_PART_HORSE,                    //坐骑
        BODY_PART_MAX,
    };
}

//容器栏
enum EItemBar
{
	pos_invalid = -1,
	pos_equipbar,			// 装备栏
	pos_itembar,			// 道具栏
	pos_num
};


/////////////////////////
//Actor 模型方面枚举
/////////////////////////
UENUM(BlueprintType)
namespace EActorUnit
{
    enum Type
    {
        eActorUnit,			    //用于实际控制的场景单位//本NPC的模型
		eRideUnit,				//坐骑
        eEquipModel,			//装备界面模型
        eActorUnitCount			//总数
    };
}

////////////////////////
//NPC方面
///////////////////////
//NPC状态
UENUM(BlueprintType)
namespace ENpcAIState
{
    enum Type
    {
        none = 0,		//	无
        guard,		    //	守卫
        follow,		    //	跟随
        combat,		    //	战斗
        flee,			//	逃跑
        patrol,		    //	巡逻
        trapped,		//	无法行动
        come_back,		//	回归
        follow_wait,	//	跟随等待(等待跟随目标出现)
        stay_forever,	//	更本无法移动(任何情况下)
        escape,		    //	逃跑
        justmove,		//	朝向目标移动
        run_to_do,	    //  跑到指定目标处执行任务
        move_search,	//	朝制定目标方向找敌人
        count
    };
}
