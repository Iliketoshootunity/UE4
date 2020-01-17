#pragma once
#include "Engine/GenericPlatform/SGenericPlatformMisc.h"

struct SLinuxPlatformMisc : public SGenericPlatformMisc
{
    //---------------------------------------------------------------------------
    // 函数:	DebugLog
    // 功能:	向调试窗口输出调试信息
    // 参数:	Fmt		格式化字符串
    //			...		输出的字符串
    // 返回:	void
    //---------------------------------------------------------------------------
    static void EG_DebugLog(const char* Fmt, ...) {}

    //---------------------------------------------------------------------------
    // 函数:	MessageBox
    // 功能:	Display a System Message Box
    // 参数:	char* lpMsg, ...
    // 返回:	void
    //---------------------------------------------------------------------------
    static void EG_MessageBox(const char* lpMsg, ...) {}

    //---------------------------------------------------------------------------
    // 函数:	AssertFailed
    // 功能:	断言失败
    // 参数:	FileName	在哪个文件中失败
    //			LineNum		在文件中的第几行
    // 返回:	void
    //---------------------------------------------------------------------------
    static void EG_AssertFailed(char* pFileName, int nLineNum);

	static FORCEINLINE char* EG_Gets(char* pData, int32 nSize)
	{
		return fgets(pData, nSize, stdin);
	}

};

typedef SLinuxPlatformMisc SPlatformMisc;