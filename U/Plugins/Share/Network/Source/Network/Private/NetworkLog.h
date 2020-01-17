#pragma once
#include "CoreMinimal.h"

#define USE_ENGINE !IS_PROGRAM && !UE_SERVER

#if USE_ENGINE
#include "Engine.h"
DECLARE_LOG_CATEGORY_EXTERN(LogNetwork, Verbose, All);
#else
DECLARE_LOG_CATEGORY_EXTERN(LogNetwork, Display, All);
#endif

template <typename FmtType, typename... Types>
static void Log(const FmtType& Text, Types... Args)
{
	FString NewLogs= FString::Printf(Text, Args...);
	
#if USE_ENGINE
	UE_LOG(LogNetwork, Verbose, TEXT("NetworkLog : %s"), *NewLogs);
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, NewLogs);
#else
	UE_LOG(LogNetwork, Display, TEXT("NetworkLog : %s"), *NewLogs);
#endif

}

static void NetLog(const char* logStr)
{
	Log(TEXT("%s"), ANSI_TO_TCHAR(logStr));
}


