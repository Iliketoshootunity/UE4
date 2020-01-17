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
        Left,                       //左侧
        Top,                        //上侧
        Right,                      //右侧
        Bottom,                     //下侧
        LeftAndTop,                 //左上
        LeftAndBottom,              //左下
        RightAndTop,                //右上
        RightAndBottom,             //右下
        Num,
    };
}