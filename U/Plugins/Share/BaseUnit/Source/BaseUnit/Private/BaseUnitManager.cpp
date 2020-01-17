// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseUnitManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/CommandLine.h"
#include "SIniFile.h"
#include "TabFile.h"

BaseUnitManager::BaseUnitManager() : 
	m_pIniFile(NULL)
	, m_pTabFile(NULL)
{
	m_pIniFile = new SIniFile();
	m_pTabFile = new FTabFile();
}

BaseUnitManager::~BaseUnitManager()
{
	if (m_pIniFile)
	{
		delete m_pIniFile;
		m_pIniFile = NULL;
	}
		

	if (m_pTabFile)
	{
        UnLoadTab();
		delete m_pTabFile;
		m_pTabFile = NULL;
	}
		
}

//BaseUnitManager* BaseUnitManager::Get()
//{
//	static BaseUnitManager m_Singleton;
//	return &m_Singleton;
//}

bool BaseUnitManager::LoadIni(const TCHAR* BaseIniName, const TCHAR* Platform /*= NULL*/, bool bForceReload /*= false*/, bool bRequireDefaultIni /*= false*/, bool bAllowGeneratedIniWhenCooked /*= true*/, const TCHAR* GeneratedConfigDir /*= NULL*/)
{
	return m_pIniFile->Load(BaseIniName, Platform, bForceReload, bRequireDefaultIni, bAllowGeneratedIniWhenCooked, GeneratedConfigDir);
}

bool BaseUnitManager::LoadIni(FString FileName, const TCHAR* Platform /*= NULL*/, bool bForceReload /*= false*/, bool bRequireDefaultIni /*= false*/, bool bAllowGeneratedIniWhenCooked /*= true*/, const TCHAR* GeneratedConfigDir /*= NULL*/)
{
    return m_pIniFile->Load(FileName, Platform, bForceReload, bRequireDefaultIni, bAllowGeneratedIniWhenCooked, GeneratedConfigDir);
}

void BaseUnitManager::UnLoadIni()
{
	m_pIniFile->UnLoad();
}

bool BaseUnitManager::GetIniString(const TCHAR* Section, const TCHAR* Key, FString lpDefault, FString& Value)
{
	return m_pIniFile->GetString(Section, Key, lpDefault, Value);
}

bool BaseUnitManager::GetIniInt(const TCHAR* Section, const TCHAR* Key, int32 nDefault, int32& Value)
{
	return m_pIniFile->GetInt(Section, Key, nDefault, Value);
}

bool BaseUnitManager::GetIniFloat(const TCHAR* Section, const TCHAR* Key, float fDefault, float& Value)
{
	return m_pIniFile->GetFloat(Section, Key, fDefault, Value);
}

bool BaseUnitManager::GetIniDouble(const TCHAR* Section, const TCHAR* Key, double dDefault, double& Value)
{
	return m_pIniFile->GetDouble(Section, Key, dDefault, Value);
}

bool BaseUnitManager::GetIniBool(const TCHAR* Section, const TCHAR* Key, bool bDefault, bool& Value)
{
	return m_pIniFile->GetBool(Section, Key, bDefault, Value);
}


int32 BaseUnitManager::GetIniArray(const TCHAR* Section, const TCHAR* Key, TArray<FString>& Value) const
{
	return m_pIniFile->GetArray(Section, Key, Value);
}

FConfigSection* BaseUnitManager::GetIniSectionPrivate(const TCHAR* Section, bool Force, bool Const)
{
	return m_pIniFile->GetSectionPrivate(Section, Force, Const);
}

bool BaseUnitManager::GetIniSectionKeyMatchValue(const TCHAR* Section, bool Force, bool Const, const TCHAR* Key, const TCHAR* Match, FName& Value)
{
	return m_pIniFile->GetSectionKeyMatchValue(Section, Force, Const, Key, Match, Value);
}

/////////////////////////////////////////////
int BaseUnitManager::m_TabRowOffest = 2;

bool BaseUnitManager::LoadTab(FString FileName)
{
	return m_pTabFile->Load(FileName);
}

int BaseUnitManager::FindTabRow(char* szRow)
{
	return m_pTabFile->FindRow(szRow);
}

int BaseUnitManager::FindTabColumn(char* szColumn)
{
	return m_pTabFile->FindColumn(szColumn);
}

int BaseUnitManager::GetTabWidth()
{
	return m_pTabFile->GetWidth();
}

int BaseUnitManager::GetTabHeight()
{
    //最上面2行，一行位中文，一行位英文，第三行才是数据的开始
	return m_pTabFile->GetHeight() - m_TabRowOffest;
}

int BaseUnitManager::GetBeginRow()
{
    return m_TabRowOffest;
}

bool BaseUnitManager::GetTabString(int nRow, char* szColumn, char* lpDefault, char* lpRString, unsigned long dwSize, bool bColumnLab /*= true*/)
{
	return m_pTabFile->GetString(nRow, szColumn, lpDefault, lpRString, dwSize, bColumnLab);
}

bool BaseUnitManager::GetTabString(int nRow, int nColumn, char* lpDefault, char* lpRString, unsigned long dwSize)
{
	return m_pTabFile->GetString(nRow, nColumn, lpDefault, lpRString, dwSize);
}

bool BaseUnitManager::GetTabString(char* szRow, char* szColumn, char* lpDefault, char* lpRString, unsigned long dwSize)
{
	return m_pTabFile->GetString(szRow, szColumn, lpDefault, lpRString, dwSize);
}

bool BaseUnitManager::GetTabString(int32 nRow, int32 nColumn, FString lpDefault, FString& lpRString)
{
    lpRString.Empty();

    char szBuffer[MAX_BUFFER_LEN] = { 0 };
    bool bResult = m_pTabFile->GetString(nRow, nColumn, TCHAR_TO_ANSI(*lpDefault), szBuffer, sizeof(szBuffer));
    lpRString = bResult ? UTF8_TO_TCHAR(szBuffer) : lpDefault;
    return  bResult;
}

bool BaseUnitManager::GetTabInteger(int nRow, char* szColumn, int nDefault, int *pnValue, bool bColumnLab /*= true*/)
{
	return m_pTabFile->GetInteger(nRow, szColumn, nDefault, pnValue, bColumnLab);
}

bool BaseUnitManager::GetTabInteger(int nRow, int nColumn, int nDefault, int *pnValue)
{
	return m_pTabFile->GetInteger(nRow, nColumn, nDefault, pnValue);
}

bool BaseUnitManager::GetTabInteger(char* szRow, char* szColumn, int nDefault, int *pnValue)
{
	return m_pTabFile->GetInteger(szRow, szColumn, nDefault, pnValue);
}

bool BaseUnitManager::GetTabInteger(int32 nRow, int32 nColumn, int32 nDefault, int32 &pnValue)
{
    return m_pTabFile->GetInteger(nRow, nColumn, nDefault, &pnValue);
}

bool BaseUnitManager::GetTabFloat(int nRow, char* szColumn, float fDefault, float *pfValue, bool bColumnLab /*= true*/)
{
	return m_pTabFile->GetFloat(nRow, szColumn, fDefault, pfValue, bColumnLab);
}

bool BaseUnitManager::GetTabFloat(int nRow, int nColumn, float fDefault, float *pfValue)
{
	return m_pTabFile->GetFloat(nRow, nColumn, fDefault, pfValue);
}

bool BaseUnitManager::GetTabFloat(char* szRow, char* szColumn, float fDefault, float *pfValue)
{
	return m_pTabFile->GetFloat(szRow, szColumn, fDefault, pfValue);
}

bool BaseUnitManager::GetTabFloat(int32 nRow, int32 nColumn, float fDefault, float &pfValue)
{
    return m_pTabFile->GetFloat(nRow, nColumn, fDefault, &pfValue);
}

bool BaseUnitManager::GetTabBool(int nRow, char* szColumn, bool bDefault, bool *pbValue, bool bColumnLab /*= true*/)
{
    return m_pTabFile->GetInteger(nRow, szColumn, (int)bDefault, (int*)pbValue, bColumnLab);
}

bool BaseUnitManager::GetTabBool(int nRow, int nColumn, bool bDefault, bool *pbValue)
{
    return m_pTabFile->GetInteger(nRow, nColumn, (int)bDefault, (int*)pbValue);
}

bool BaseUnitManager::GetTabBool(char* szRow, char* szColumn, bool bDefault, bool *pbValue)
{
    return m_pTabFile->GetInteger(szRow, szColumn, (int)bDefault, (int*)pbValue);
}

bool BaseUnitManager::GetTabBool(int32 nRow, int32 nColumn, bool bDefault, bool &pbValue)
{
    return m_pTabFile->GetInteger(nRow, nColumn, (int32)bDefault, (int32*)&pbValue);
}

bool BaseUnitManager::GetTabMaterial(int32 nRow, int32 nColumn, FString Format, FString lpDefault, FMaterialData& outMaterialData)
{
    FPlatformMemory::Memzero(&outMaterialData, sizeof(outMaterialData));

    FString lpMaterial;
    bool bResult = GetTabString(nRow, nColumn, lpDefault, lpMaterial);
    if ((!bResult || lpMaterial.IsEmpty()) && !lpDefault.IsEmpty())
    {
        FString Left;
        lpDefault.Split(Format, &Left, &outMaterialData.MaterialName);
        outMaterialData.nSlot = FCString::Atoi(*Left);
        return false;
    }

    FString Left;
    lpMaterial.Split(Format, &Left, &outMaterialData.MaterialName);
    outMaterialData.nSlot = FCString::Atoi(*Left);
    return true;
}

void BaseUnitManager::UnLoadTab()
{
	m_pTabFile->Clear();
}
