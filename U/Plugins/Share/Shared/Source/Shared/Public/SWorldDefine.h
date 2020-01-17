#pragma once
#include "CoreMinimal.h"
#include "SWorldDefine.generated.h"

#define INVALID_REGION_INDEX -1
#define INVALID_WORLD_INDEX -1

UENUM(BlueprintType)
namespace ENineGridPos
{
    enum Type
    {
        None = -1,
        Center,
        Left,                       //���
        Top,                        //�ϲ�
        Right,                      //�Ҳ�
        Bottom,                     //�²�
        LeftAndTop,                 //����
        LeftAndBottom,              //����
        RightAndTop,                //����
        RightAndBottom,             //����
        Num,
    };
}