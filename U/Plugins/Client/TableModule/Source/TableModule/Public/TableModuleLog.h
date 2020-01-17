#pragma once
#include "Engine/Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTableModule, Verbose, All);

static void Log(FString LogString, bool bLogOnScreen /*= false*/, float fShowTime /*= 5.0f*/, FColor fShowColor /*= FColor::Red*/)
{
    UE_LOG(LogTableModule, Verbose, TEXT("LogNetUnit : %s"), *LogString);

    if (bLogOnScreen && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, fShowTime, fShowColor, LogString);
    }
}

template <typename FmtType, typename... Types>
static void Logs(const FmtType& LogString, bool bLogOnScreen, float fShowTime, FColor fShowColor, Types... Args)
{
    FString _LogString = FString::Printf(LogString, Args...);
    Log(_LogString, bLogOnScreen, fShowTime, fShowColor);
}