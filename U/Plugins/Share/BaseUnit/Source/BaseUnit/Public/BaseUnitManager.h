// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefine.h"
#include "BasicFunction.h"

/**
 *  不支持多线程（多线程，单独调用STabFile,SiniFile），只支持单线程
 */
class SIniFile;
class FTabFile;

class BASEUNIT_API BaseUnitManager
{
	
public:
	BaseUnitManager();
	~BaseUnitManager();

	//static BaseUnitManager*	Get();
    //带路径的FileName
    bool                    LoadIni(FString FileName, const TCHAR* Platform = NULL, bool bForceReload = false, bool bWriteDestIni = false, bool bAllowGeneratedIniWhenCooked = true, const TCHAR* GeneratedConfigDir = NULL);
    //不带路径的FileName
	bool					LoadIni(const TCHAR* BaseIniName, const TCHAR* Platform = NULL, bool bForceReload = false, bool bRequireDefaultIni = false, bool bAllowGeneratedIniWhenCooked = true, const TCHAR* GeneratedConfigDir = NULL);
	void					UnLoadIni();
	bool					GetIniString(const TCHAR* Section, const TCHAR* Key, FString lpDefault, FString& Value);
	bool					GetIniInt(const TCHAR*	Section, const TCHAR* Key, int32 nDefault, int32& Value);
	bool					GetIniFloat(const TCHAR* Section, const TCHAR* Key, float fDefault, float& Value);
	bool					GetIniDouble(const TCHAR* Section, const TCHAR* Key, double dDefault, double& Value);
	bool					GetIniBool(const TCHAR* Section, const TCHAR* Key, bool bDefault, bool& Value);
	int32					GetIniArray(const TCHAR* Section, const TCHAR* Key, TArray<FString>& Value) const;
	FConfigSection*			GetIniSectionPrivate(const TCHAR* Section, bool Force, bool Const);
	bool					GetIniSectionKeyMatchValue(const TCHAR* Section, bool Force, bool Const, const TCHAR* Key, const TCHAR* Match, FName& Value);

	//Tab Files Functions
	bool					LoadTab(FString FileName);
	int						FindTabRow(char* szRow);//Return value starting with 1
	int						FindTabColumn(char* szColumn);//Return value starting with 1
	int						GetTabWidth();
	int						GetTabHeight();
    int                     GetBeginRow();
	bool					GetTabString(int nRow, char* szColumn, char* lpDefault, char* lpRString, unsigned long dwSize, bool bColumnLab = true);
	bool					GetTabString(int nRow, int nColumn, char* lpDefault, char* lpRString, unsigned long dwSize);
	bool					GetTabString(char* szRow, char* szColumn, char* lpDefault, char* lpRString, unsigned long dwSize);
    bool					GetTabString(int32 nRow, int32 nColumn, FString lpDefault, FString& lpRString);

	template<typename... Types>
	bool					GetTabString(int32 nRow, int32 nColumn, FString lpDefault, FString Format, Types... Args)
	{
		FString FSValue;

		bool result = GetTabString(nRow, nColumn, lpDefault, FSValue);

		if (FSValue.IsEmpty())
			return result;

		FBasicFunction::ue_sscanf(FSValue, TCHAR_TO_ANSI(*Format), Args...);

		return result;
	}

	bool					GetTabInteger(int nRow, char* szColumn, int nDefault, int *pnValue, bool bColumnLab = true);
	bool					GetTabInteger(int nRow, int nColumn, int nDefault, int *pnValue);
	bool					GetTabInteger(char* szRow, char* szColumn, int nDefault, int *pnValue);
    bool					GetTabInteger(int32 nRow, int32 nColumn, int32 nDefault, int32 &pnValue);
	bool					GetTabFloat(int nRow, char* szColumn, float fDefault, float *pfValue, bool bColumnLab = true);
	bool					GetTabFloat(int nRow, int nColumn, float fDefault, float *pfValue);
	bool					GetTabFloat(char* szRow, char* szColumn, float fDefault, float *pfValue);
    bool					GetTabFloat(int32 nRow, int32 nColumn, float fDefault, float &pfValue);
    bool					GetTabBool(int nRow, char* szColumn, bool bDefault, bool *pbValue, bool bColumnLab = true);
    bool					GetTabBool(int nRow, int nColumn, bool bDefault, bool *pbValue);
    bool					GetTabBool(char* szRow, char* szColumn, bool bDefault, bool *pbValue);
    bool					GetTabBool(int32 nRow, int32 nColumn, bool bDefault, bool &pbValue);

    bool					GetTabMaterial(int32 nRow, int32 nColumn,FString Format, FString lpDefault, FMaterialData& outMaterialData);
	void					UnLoadTab();
private:
	SIniFile*				m_pIniFile;
	FTabFile*				m_pTabFile;

private:
    static  int             m_TabRowOffest;
};

