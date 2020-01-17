#include "SIniFile.h"
#include "Misc/CommandLine.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/Paths.h"
#include "BaseUnitLog.h"


SIniFile::SIniFile():
	m_FileName(TEXT(""))
	, m_IsLoaded(false)
{
	m_GameConfig = new FConfigCacheIni(EConfigCacheType::DiskBacked);
}

SIniFile::~SIniFile()
{
	if (m_GameConfig)
	{
		m_GameConfig->Exit();
		delete m_GameConfig;
		m_GameConfig = NULL;
	}
}

FString SIniFile::GetDestIniFilename(const TCHAR* BaseIniName, const TCHAR* PlatformName, const TCHAR* GeneratedConfigDir, FString* IniName, FString* FileType)
{
	// figure out what to look for on the commandline for an override
	FString CommandLineSwitch = FString::Printf(TEXT("%sINI="), BaseIniName);


	// if it's not found on the commandline, then generate it
	FString IniFilename;
	if (FParse::Value(FCommandLine::Get(), *CommandLineSwitch, IniFilename) == false)
	{
		FString Name(PlatformName ? PlatformName : ANSI_TO_TCHAR(FPlatformProperties::PlatformName()));

		FString BaseIniNameString = BaseIniName;
		if (BaseIniNameString.Contains(GeneratedConfigDir))
		{
			IniFilename = BaseIniNameString;
		}
		else
		{
			if (BaseIniNameString.Contains(TEXT(".ini")))
			{
				IniFilename = FString::Printf(TEXT("%s%s/%s"), GeneratedConfigDir, *Name, BaseIniName);
			}
			else
			{
				
				// put it all together
				IniFilename = FString::Printf(TEXT("%s%s/%s.ini"), GeneratedConfigDir, *Name, BaseIniName);
			}
			
		}

		BaseIniNameString.Split(TEXT("."), IniName, FileType);
	}

	// standardize it!
	FPaths::MakeStandardFilename(IniFilename);
	return IniFilename;
}

bool SIniFile::Load(const TCHAR* BaseIniName, const TCHAR* Platform, bool bForceReload, bool bRequireDefaultIni, bool bAllowGeneratedIniWhenCooked, const TCHAR* GeneratedConfigDir)
{
	FString m_IniName, m_FileType;
	// figure out where the end ini file is
	m_FileName = GetDestIniFilename(BaseIniName, Platform, GeneratedConfigDir,&m_IniName,&m_FileType);

	// need to check to see if the file already exists in the GConfigManager's cache
	// if it does exist then we are done, nothing else to do
	if (!bForceReload && m_GameConfig->FindConfigFile(*m_FileName) != nullptr)
	{
		//UE_LOG(LogConfig, Log,  TEXT( "Request to load a config file that was already loaded: %s" ), GeneratedIniFile );
		return true;
	}

	// make a new entry in GConfig (overwriting what's already there)
	FConfigFile& NewConfigFile = m_GameConfig->Add(m_FileName, FConfigFile());

	if (m_IniName.IsEmpty())
	{
		m_IsLoaded = FConfigCacheIni::LoadExternalIniFile(NewConfigFile, BaseIniName, *FPaths::EngineConfigDir(), *FPaths::SourceConfigDir(), true, Platform, bForceReload, true, bAllowGeneratedIniWhenCooked, GeneratedConfigDir);
	}
	else
	{
		m_IsLoaded = FConfigCacheIni::LoadExternalIniFile(NewConfigFile, *m_IniName, *FPaths::EngineConfigDir(), *FPaths::SourceConfigDir(), true, Platform, bForceReload, true, bAllowGeneratedIniWhenCooked, GeneratedConfigDir);
	}

	if(!m_IsLoaded)
		UE_LOG(Gamelog, Log, TEXT("load result=%d"), m_IsLoaded);

	return m_IsLoaded;
}

bool SIniFile::Load(FString FileName, const TCHAR* Platform /*= NULL*/, bool bForceReload /*= false*/, bool bWriteDestIni /*= false*/, bool bAllowGeneratedIniWhenCooked /*= true*/, const TCHAR* GeneratedConfigDir /*= *FPaths::GeneratedConfigDir()*/)
{
    if (FileName.IsEmpty()) return false;

    FString ConfigDir;
    ConfigDir = GeneratedConfigDir == NULL ? FPaths::ProjectContentDir() : GeneratedConfigDir;

    FString szPathFileName = FString::Format(TEXT("{0}{1}"), { ConfigDir ,FileName });

    m_FileName = FPaths::ConvertRelativePathToFull(szPathFileName);

    //如果不为空，则已经加载过，并在内存中
    if (!bForceReload &&  m_GameConfig->FindConfigFile(*m_FileName) != nullptr)
    {
        //UE_LOG(LogConfig, Log,  TEXT( "Request to load a config file that was already loaded: %s" ), GeneratedIniFile );
        return true;
    }

    FString IniName, FileType;
    FileName.Split(TEXT("."), &IniName, &FileType);
    FString _Platform;
    IniName.Split(TEXT("/"), &_Platform, &IniName);
    _Platform = Platform == NULL ? _Platform : Platform;

    FConfigFile& NewConfigFile = m_GameConfig->Add(m_FileName, FConfigFile());

    m_IsLoaded = FConfigCacheIni::LoadExternalIniFile(NewConfigFile, *IniName, *FPaths::EngineConfigDir(), *FPaths::SourceConfigDir(), true,
        *_Platform, bForceReload, bWriteDestIni, bAllowGeneratedIniWhenCooked, *ConfigDir);


    if (!m_IsLoaded)
        UE_LOG(Gamelog, Log, TEXT("load result=%d"), m_IsLoaded);

    return m_IsLoaded;
}


bool SIniFile::GetString(const TCHAR* Section, const TCHAR* Key, FString lpDefault, FString& Value)
{
	if (!m_GameConfig && !m_IsLoaded)
	{
		Value = lpDefault;
		return false;
	}

	if (!m_GameConfig->GetString(Section, Key, Value, m_FileName))
	{
		Value = lpDefault;
		return false;
	}

	return true;
}

bool SIniFile::GetText(const TCHAR* Section, const TCHAR* Key, FText lpDefault, FText& Value) const
{
	if (!m_GameConfig && !m_IsLoaded)
	{
		Value = lpDefault;
		return false;
	}

	if (!m_GameConfig->GetText(Section, Key, Value, m_FileName))
	{
		Value = lpDefault;
		return false;
	}

	return true;
}

bool SIniFile::GetInt(const TCHAR*	Section, const TCHAR* Key, int32 nDefault, int32& Value)
{
	FString Text;
	if (GetString(Section, Key, TEXT(""), Text))
	{
		Value = FCString::Atoi(*Text);
		return true;
	}

	Value = nDefault;

	return false;
}

bool SIniFile::GetFloat(const TCHAR* Section, const TCHAR* Key, float fDefault, float& Value)
{
	FString Text;
	if (GetString(Section, Key, TEXT(""), Text))
	{
		Value = FCString::Atof(*Text);
		return true;
	}

	Value = fDefault;

	return false;
}

bool SIniFile::GetDouble(const TCHAR* Section, const TCHAR* Key, double dDefault, double& Value)
{
	FString Text;
	if (GetString(Section, Key, TEXT(""), Text))
	{
		Value = FCString::Atod(*Text);
		return true;
	}

	Value = dDefault;

	return false;
}

bool SIniFile::GetBool(const TCHAR* Section, const TCHAR* Key, bool bDefault, bool& Value)
{
	FString Text;
	if (GetString(Section, Key, TEXT(""), Text))
	{
		Value = FCString::ToBool(*Text);
		return true;
	}

	Value = bDefault;

	return false;
}

int32 SIniFile::GetArray(const TCHAR* Section, const TCHAR* Key, TArray<FString>& Value) const
{
	int32 ArrayNum = 0;
	if (!m_GameConfig && !m_IsLoaded)
	{
		Value.Empty();
		return ArrayNum;
	}

	ArrayNum = m_GameConfig->GetArray(Section, Key, Value, m_FileName);

	return ArrayNum;
}

FConfigSection* SIniFile::GetSectionPrivate(const TCHAR* Section, bool Force, bool Const)
{
	if (!m_GameConfig && !m_IsLoaded)
	{
		return NULL;
	}

	return m_GameConfig->GetSectionPrivate(Section, Force, Const, m_FileName);
}

bool SIniFile::GetSectionKeyMatchValue(const TCHAR* Section, bool Force, bool Const, const TCHAR* Key, const TCHAR* Match, FName& Value)
{
	Value = NAME_Object;
	FConfigSection* pSectionList = GetSectionPrivate(Section, Force, Const);
	for (FConfigSection::TIterator It(*pSectionList); It; ++It)
	{
		if (It.Key() == Key)
		{
			FParse::Value(*It.Value().GetValue(), Match, Value);
			break;
		}
	}

	return Value == NAME_Object ? false : true;
}

void SIniFile::UnLoad()
{
	if (m_GameConfig->FindConfigFile(m_FileName) != nullptr)
		m_GameConfig->Remove(m_FileName);

	m_FileName = TEXT("");
	m_IsLoaded = false;
}
