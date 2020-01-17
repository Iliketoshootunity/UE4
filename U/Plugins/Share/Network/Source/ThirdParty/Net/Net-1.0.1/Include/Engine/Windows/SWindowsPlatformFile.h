#pragma once
#include "Engine/GenericPlatform/SGenericPlatformFile.h"


struct SWindowsPlatformFile : public SGenericPlatformFile
{
public:
	virtual ~SWindowsPlatformFile() {}
public:
    //---------------------------------------------------------------------------
    // 函数:	Open
    // 功能:	打开一个文件，准备读取
    // 参数:	FileName	文件名
    // 返回:	成功返回TRUE，失败返回FALSE。
    //---------------------------------------------------------------------------
    virtual bool	Open(char* FileName) override;
};

typedef SWindowsPlatformFile SFile;
