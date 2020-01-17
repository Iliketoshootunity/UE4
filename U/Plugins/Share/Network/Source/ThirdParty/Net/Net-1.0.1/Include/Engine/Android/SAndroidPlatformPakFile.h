#pragma once
#include "Engine/GenericPlatform/SGenericPlatformPakFile.h"


struct SAndroidPlatformPakFile : public SGenericPlatformPakFile
{
public:
    virtual bool        Open(const char* pszFileName)  override;
};

typedef SAndroidPlatformPakFile SPakFile;