#pragma once
#include "Engine/GenericPlatform/SGenericPlatformPath.h"

struct SAndroidPlatformPath : public SGenericPlatformPath
{
public:
	SAndroidPlatformPath();
	virtual ~SAndroidPlatformPath() {}
public:
    static void SetRootPath(char* lpPathName);
    static void GetFullPath(char* lpPathName, char* lpFileName);

    static int RemoveTwoPointPath(char* szPath, int nLength);
    static int RemoveOnePointPath(char* szPath, int nLength);
    static int RemoveAllPointPath(char* szPath, int nLength);

    static uint32 FileName2Id(char* lpFileName);
};

typedef SAndroidPlatformPath  SPath;
