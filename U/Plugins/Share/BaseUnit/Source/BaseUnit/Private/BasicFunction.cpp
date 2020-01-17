#include "BasicFunction.h"
#include "BaseUnitLog.h"

void FBasicFunction::Log(FString LogString, bool bLogOnScreen /*= false*/, float fShowTime /*= 5.0f*/, FColor fShowColor /*= FColor::Red*/)
{
#if LOG_SHOW
#if USE_ENGINE
	UE_LOG(Gamelog, Verbose, TEXT("Game Log : %s"), *LogString);
	if (bLogOnScreen)
	{
		GEngine->AddOnScreenDebugMessage(-1, fShowTime, fShowColor, LogString);
	}
#else
	UE_LOG(Gamelog, Display, TEXT("Game Log : %s"), *LogString);
#endif
#endif
}

int16 FBasicFunction::MakeU16(int8 a, int8 b)
{
	return ((int16)(((uint8)(a)) | ((uint16)((uint8)(b))) << 8));
}

uint8 FBasicFunction::SplitLoU16(int16 w)
{
	return ((uint8)(w));
}

uint8 FBasicFunction::SplitHiU16(int16 w)
{
	return ((uint8)(((uint16)(w) >> 8) & 0xFF));
}

int32 FBasicFunction::MakeU32(int16 a, int16 b)
{
	return ((int32)(((uint16)(a)) | ((uint32)((uint16)(b))) << 16));
}

uint16 FBasicFunction::SplitLoU32(int32 w)
{
	return ((uint16)(w));
}

uint16 FBasicFunction::SplitHiU32(int32 w)
{
	return ((uint16)(((uint32)(w) >> 16) & 0xFFFF));
}

int64 FBasicFunction::MakeU64(int32 a, int32 b)
{
	return ((int64)(((uint32)(a)) | ((uint64)((uint32)(b))) << 32));
}

uint32 FBasicFunction::SplitLoU64(int64 w)
{
	return ((uint32)(w));
}

uint32 FBasicFunction::SplitHiU64(int64 w)
{
	return ((uint32)(((uint64)(w) >> 32) & 0xFFFFFFFF));
}
