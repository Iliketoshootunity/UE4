#pragma once
#include "Engine/GenericPlatform/SGenericPlatformMisc.h"

struct SLinuxPlatformMisc : public SGenericPlatformMisc
{
    //---------------------------------------------------------------------------
    // ����:	DebugLog
    // ����:	����Դ������������Ϣ
    // ����:	Fmt		��ʽ���ַ���
    //			...		������ַ���
    // ����:	void
    //---------------------------------------------------------------------------
    static void EG_DebugLog(const char* Fmt, ...) {}

    //---------------------------------------------------------------------------
    // ����:	MessageBox
    // ����:	Display a System Message Box
    // ����:	char* lpMsg, ...
    // ����:	void
    //---------------------------------------------------------------------------
    static void EG_MessageBox(const char* lpMsg, ...) {}

    //---------------------------------------------------------------------------
    // ����:	AssertFailed
    // ����:	����ʧ��
    // ����:	FileName	���ĸ��ļ���ʧ��
    //			LineNum		���ļ��еĵڼ���
    // ����:	void
    //---------------------------------------------------------------------------
    static void EG_AssertFailed(char* pFileName, int nLineNum);

	static FORCEINLINE char* EG_Gets(char* pData, int32 nSize)
	{
		return fgets(pData, nSize, stdin);
	}

};

typedef SLinuxPlatformMisc SPlatformMisc;