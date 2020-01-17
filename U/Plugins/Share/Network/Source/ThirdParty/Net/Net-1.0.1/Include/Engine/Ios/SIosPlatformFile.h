#pragma once
#include "Engine/GenericPlatform/SGenericPlatformFile.h"

struct SIosPlatformFile : public SGenericPlatformFile
{
public:
	virtual ~SIosPlatformFile() {}

public:
    //---------------------------------------------------------------------------
    // ����:	Open
    // ����:	��һ���ļ���׼����ȡ
    // ����:	FileName	�ļ���
    // ����:	�ɹ�����TRUE��ʧ�ܷ���FALSE��
    //---------------------------------------------------------------------------
    virtual bool	Open(char* FileName) override;
};

typedef SIosPlatformFile SFile;