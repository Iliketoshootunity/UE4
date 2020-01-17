#pragma once

#include "CoreMinimal.h"

#define USE_ENGINE !IS_PROGRAM && !UE_SERVER

#define     GENERATED_SUPER(c) public :  typedef c Super;
#define		ANSI_NONE TCHAR_TO_ANSI(TEXT(""))
#define     ANSI_NONE_PARAM(s) TCHAR_TO_ANSI(TEXT(s))
#define     NONE_INDEX 0
#define     INVALID_INDEX -1
#define     MAX_RES_STRING_LEN 128
#define     MAX_BUFFER_LEN  256
#define     MAX_DEFAULT_UNIQUEID 512


typedef struct tagMaterialData
{
    int32       nSlot;                  //材质插槽索引
    FString     MaterialName;           //材质资源路径+文件名
}FMaterialData;

