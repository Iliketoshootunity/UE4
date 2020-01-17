// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// ... add public include paths required here ...

#include "SGameDefine.generated.h"



// ... add public Defines required here ...
#define MAX_ACCOUNT		32
#define MAX_PASSWORD	64
#define MAX_NAME_LEN	32


//���紫����������
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
//�������� ItemGener
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

//���߾������ ItemDetail
UENUM(BlueprintType)
namespace ItemDetail
{
    enum Type
    {
        item_invalid = -1,
        item_helm,                  //ͷ��0
        item_armor,                 //����1
        item_shoulder,              //����2
        item_belt,                  //����3
		item_leg,					//��4
		item_weapon,				//����5
		item_ride,					//����6
        item_number,
    };
}

//������ϸ��� ItemIndex
//������ϸ���Ϊ����index [0 ... max ]

//������С��� ItemMinCategory
/*weapon�� ���ࣨ����1��ȭ�ף�2���٣�4��������8 ��
(����ʱ����˵�� �������ñ� �� 1 ������2��ȭ�ף���3����+ȭ�ף���4���٣���5����+�٣���6����+ȭ�ף�
��7����+��+ȭ�ף���8����������9������+������10������+ȭ�ף���11������+��+ȭ�ף�,12(����+��)��13������+��+�٣���14������+ȭ��+�٣���15������+��+ȭ��+�٣���
�÷��������ܶ����������ƣ�WeaponSkill & nMinCategory ����С���
*/
//��������С�����Ԥ��


//װ����С���,������ɫװ��λ
UENUM(BlueprintType)
namespace EquipPart
{
    enum Type
    {
        part_invalid = -1,
        part_main_weapon,	// װ������1 Weapon ����part [0�������ܰ��ţ�1��part_main_weapon��2��part_weapon1��3����part_main_weapon��part_weapon2����4��part_weapon2]
        part_weapon1,	// װ������2    [5��(part_main_weapon,part_weapon2), 6:(part_weapon1,part_weapon2), 7:(part_main_weapon,part_weapon1,part_weapon2)]
        part_weapon2,	// װ������3
        part_helm,		// װ��ͷ�� 4
        part_num		// װ����λ��Ŀ
    };
}


/************************************************************************
* Desc 	: BodyPart.txt��Ӧ�Ĳ���ģ����Դ��Item-Mesh res��
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
        BODY_PART_HAIR,                     //ͷ����ͷ��
		BODY_PART_FACE,                     //��
		BODY_PART_SHOULDER,                 //��
        BODY_PART_BODY,                     //���� 
        BODY_PART_BELT,                     //��
        BODY_PART_LEG,                      //��
		BODY_PART_LHAND_WEAPON,             //��������
		BODY_PART_RHAND_WEAPON,				//��������
        BODY_PART_HORSE,                    //����
        BODY_PART_MAX,
    };
}

//������
enum EItemBar
{
	pos_invalid = -1,
	pos_equipbar,			// װ����
	pos_itembar,			// ������
	pos_num
};


/////////////////////////
//Actor ģ�ͷ���ö��
/////////////////////////
UENUM(BlueprintType)
namespace EActorUnit
{
    enum Type
    {
        eActorUnit,			    //����ʵ�ʿ��Ƶĳ�����λ//��NPC��ģ��
		eRideUnit,				//����
        eEquipModel,			//װ������ģ��
        eActorUnitCount			//����
    };
}

////////////////////////
//NPC����
///////////////////////
//NPC״̬
UENUM(BlueprintType)
namespace ENpcAIState
{
    enum Type
    {
        none = 0,		//	��
        guard,		    //	����
        follow,		    //	����
        combat,		    //	ս��
        flee,			//	����
        patrol,		    //	Ѳ��
        trapped,		//	�޷��ж�
        come_back,		//	�ع�
        follow_wait,	//	����ȴ�(�ȴ�����Ŀ�����)
        stay_forever,	//	�����޷��ƶ�(�κ������)
        escape,		    //	����
        justmove,		//	����Ŀ���ƶ�
        run_to_do,	    //  �ܵ�ָ��Ŀ�괦ִ������
        move_search,	//	���ƶ�Ŀ�귽���ҵ���
        count
    };
}
