#pragma once

#include "CoreMinimal.h"

class FConfigCacheIni;

class BASEUNIT_API SIniFile
{
public:
	SIniFile();
	~SIniFile();

	FString					GetDestIniFilename(const TCHAR* BaseIniName, const TCHAR* PlatformName, const TCHAR* GeneratedConfigDir, FString* IniName,FString* FileType );
	
    //FileName带路径
    bool                    Load(FString FileName, const TCHAR* Platform = NULL, bool bForceReload = false, bool bWriteDestIni = false, bool bAllowGeneratedIniWhenCooked = true, const TCHAR* GeneratedConfigDir = NULL);
    //FileName不带路径
    bool					Load(const TCHAR* BaseIniName, const TCHAR* Platform = NULL, bool bForceReload = false, bool bRequireDefaultIni = false, bool bAllowGeneratedIniWhenCooked = true, const TCHAR* GeneratedConfigDir = *FPaths::GeneratedConfigDir());

	inline FConfigCacheIni*	GetConfigCacheIni() { return m_GameConfig; }

	inline FString			GetFileName() { return m_FileName; }

	bool					GetString(const TCHAR* Section, const TCHAR* Key, FString lpDefault, FString& Value);
	bool					GetText(const TCHAR* Section, const TCHAR* Key, FText lpDefault, FText& Value) const;
	bool					GetInt(const TCHAR*	Section, const TCHAR* Key,int32 nDefault, int32& Value);
	bool					GetFloat(const TCHAR* Section,const TCHAR* Key, float fDefault, float& Value);
	bool					GetDouble(const TCHAR* Section,const TCHAR* Key, double dDefault, double& Value);
	bool					GetBool(const TCHAR* Section,const TCHAR* Key,bool bDefault, bool& Value);
	int32					GetArray(const TCHAR* Section, const TCHAR* Key, TArray<FString>& Value) const;
	FConfigSection*			GetSectionPrivate(const TCHAR* Section, bool Force, bool Const);
	bool					GetSectionKeyMatchValue(const TCHAR* Section, bool Force, bool Const, const TCHAR* Key, const TCHAR* Match, FName& Value);

	void					UnLoad();
private:
	FConfigCacheIni*	m_GameConfig;
	FString				m_FileName;
	bool				m_IsLoaded;

};

