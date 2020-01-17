#pragma once
#include "Engine/GenericPlatform/SGenericPlatformFile.h"

struct SLinuxPlatformFile : public SGenericPlatformFile
{
public:
	virtual ~SLinuxPlatformFile() {}

public:
    //---------------------------------------------------------------------------
    // ����:	Open
    // ����:	��һ���ļ���׼����ȡ
    // ����:	FileName	�ļ���
    // ����:	�ɹ�����TRUE��ʧ�ܷ���FALSE��
    //---------------------------------------------------------------------------
    virtual bool	Open(char* FileName) override;
};

typedef SLinuxPlatformFile SFile;