#pragma once
#include "Engine/GenericPlatform/SGenericPlatformFile.h"

struct SAndroidPlatformFile : public SGenericPlatformFile
{
public:
	virtual ~SAndroidPlatformFile() {}

public:
    //---------------------------------------------------------------------------
    // ����:	Open
    // ����:	��һ���ļ���׼����ȡ
    // ����:	FileName	�ļ���
    // ����:	�ɹ�����TRUE��ʧ�ܷ���FALSE��
    //---------------------------------------------------------------------------
    virtual bool	Open(char* FileName) override;
};

typedef SAndroidPlatformFile SFile;