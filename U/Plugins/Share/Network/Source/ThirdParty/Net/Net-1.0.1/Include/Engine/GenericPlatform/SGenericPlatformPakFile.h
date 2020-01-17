#pragma once
#include "Engine/HAL/SPlatform.h"
#include "Engine/HAL/SPlatformFile.h"

struct SGenericPlatformPakFile
{
public:
	SGenericPlatformPakFile(){}
	virtual ~SGenericPlatformPakFile() {};
public:
    virtual bool                    Open(const char* pszFileName) = 0;
    void		                    Close();
	SIZE_T		                    Read(void* pBuffer, SIZE_T dwSize);
	SIZE_T		                    Seek(long lOffset, uint32 dwMethod);
	SIZE_T		                    Size();
public:
    SFile		                    m_File;			// 真实文件(不在包中)
};