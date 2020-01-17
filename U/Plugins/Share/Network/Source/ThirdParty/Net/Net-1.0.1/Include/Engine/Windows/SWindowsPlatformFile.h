#pragma once
#include "Engine/GenericPlatform/SGenericPlatformFile.h"


struct SWindowsPlatformFile : public SGenericPlatformFile
{
public:
	virtual ~SWindowsPlatformFile() {}
public:
    //---------------------------------------------------------------------------
    // ����:	Open
    // ����:	��һ���ļ���׼����ȡ
    // ����:	FileName	�ļ���
    // ����:	�ɹ�����TRUE��ʧ�ܷ���FALSE��
    //---------------------------------------------------------------------------
    virtual bool	Open(char* FileName) override;
};

typedef SWindowsPlatformFile SFile;
