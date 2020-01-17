#pragma once
#include "Engine/HAL/SPlatform.h"

#ifndef MAXPATH
#define MAXPATH   260
#define MAXDIR    256
#define MAXFILE   256
#define MAXEXT    256
#define MAXDRIVE    3
#endif

struct SGenericPlatformPath
{
public:
    static char szRootPath[MAXPATH];
    static char szCurrPath[MAXPATH];

    static void GetRootPath(char* lpPathName);

    static void UnitePathAndName(char *lpPath, char *lpFile, char *lpGet);
};