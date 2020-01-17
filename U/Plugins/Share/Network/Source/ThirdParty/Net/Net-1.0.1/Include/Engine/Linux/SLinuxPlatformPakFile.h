#pragma once
#include "Engine/GenericPlatform/SGenericPlatformPakFile.h"


struct SLinuxPlatformPakFile : public SGenericPlatformPakFile
{
public:
	SLinuxPlatformPakFile() {}
	virtual ~SLinuxPlatformPakFile() {}

public:
    virtual bool        Open(const char* pszFileName)  override;
};

typedef SLinuxPlatformPakFile SPakFile;