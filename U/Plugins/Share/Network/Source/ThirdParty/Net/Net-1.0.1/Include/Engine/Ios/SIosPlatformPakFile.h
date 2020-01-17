#pragma once
#include "Engine/GenericPlatform/SGenericPlatformPakFile.h"


struct SIosPlatformPakFile : public SGenericPlatformPakFile
{
public:
	SIosPlatformPakFile() {}
	virtual ~SIosPlatformPakFile() {}

public:
    virtual bool        Open(const char* pszFileName)  override;
};

typedef SIosPlatformPakFile SPakFile;