#pragma once
#include "Engine/GenericPlatform/SGenericPlatformMisc.h"

struct SWindowsPlatformMisc : public SGenericPlatformMisc
{
    static HWND m_hWndDebug;

	static void CreateGuid(struct SGuid& Guid);

    //---------------------------------------------------------------------------
    // ����:	EG_DebugLog
    // ����:	����Դ������������Ϣ
    // ����:	Fmt		��ʽ���ַ���
    //			...		������ַ���
    // ����:	void
    //---------------------------------------------------------------------------
    static void EG_DebugLog(const char* Fmt, ...);

    //---------------------------------------------------------------------------
    // ����:	EG_MessageBox
    // ����:	Display a System Message Box
    // ����:	char* lpMsg, ...
    // ����:	void
    //---------------------------------------------------------------------------
    static void EG_MessageBox(const char* lpMsg, ...);

    //---------------------------------------------------------------------------
    // ����:	EG_AssertFailed
    // ����:	����ʧ��
    // ����:	FileName	���ĸ��ļ���ʧ��
    //			LineNum		���ļ��еĵڼ���
    // ����:	void
    //---------------------------------------------------------------------------
    static void EG_AssertFailed(char* pFileName, int nLineNum);

	static FORCEINLINE char* EG_Gets(char* pData, SIZE_T nSize)
	{
		return gets_s(pData, nSize);
	}
};

typedef SWindowsPlatformMisc SPlatformMisc;