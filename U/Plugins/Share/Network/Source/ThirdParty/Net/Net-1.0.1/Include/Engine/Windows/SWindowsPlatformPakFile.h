#pragma once
#include "Engine/GenericPlatform/SGenericPlatformPakFile.h"


struct SWindowsPlatformPakFile : public SGenericPlatformPakFile
{
public:
	SWindowsPlatformPakFile() {}
	virtual ~SWindowsPlatformPakFile() {}
public:
    virtual bool        Open(const char* pszFileName)  override;
};

typedef SWindowsPlatformPakFile SPakFile;