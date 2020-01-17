#pragma once
#include "CoreMinimal.h"


struct BASEUNIT_API FBasicFunction
{
public:
	static void Log(FString LogString, bool bLogOnScreen = false, float fShowTime = 5.0f, FColor fShowColor = FColor::Red);
	

	template <typename FmtType,typename... Types>
	static void Logs(const FmtType& Fmt, Types... Args)
	{
		FString _LogString = FString::Printf(Fmt, Args...);

#if UE_BUILD_SHIPPING
		Log(_LogString);
#else
		Log(_LogString, true);
#endif
	}

	template <typename... Types>
	static void ue_sscanf(const FString _Buffer, const char* _Format, Types... Args)
	{
#if PLATFORM_WINDOWS
		sscanf_s(TCHAR_TO_ANSI(*_Buffer), _Format, Args...);
#else
		sscanf(TCHAR_TO_ANSI(*_Buffer), _Format, Args...);
#endif
	}

	//合并两个int,生成一个int
	static int16               MakeU16(int8 a, int8 b);
	static uint8               SplitLoU16(int16 w);
	static uint8               SplitHiU16(int16 w);

	static int32               MakeU32(int16 a, int16 b);
	static uint16              SplitLoU32(int32 w);
	static uint16              SplitHiU32(int32 w);

	static int64               MakeU64(int32 a, int32 b);
	static uint32              SplitLoU64(int64 w);
	static uint32              SplitHiU64(int64 w);


};