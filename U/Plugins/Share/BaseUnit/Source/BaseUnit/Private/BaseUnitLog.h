#pragma once
#include "CoreMinimal.h"
#include "BaseDefine.h"

#if USE_ENGINE
#include "Engine.h"
DECLARE_LOG_CATEGORY_EXTERN(Gamelog, Verbose, All);
#else
DECLARE_LOG_CATEGORY_EXTERN(Gamelog, Display, All);
#endif