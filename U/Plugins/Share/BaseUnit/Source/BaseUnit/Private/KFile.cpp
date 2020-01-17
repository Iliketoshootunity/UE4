#include "KFile.h"
#include "BaseUnitDef.h"
#include "BaseUnitLog.h"
#include "Misc/FileHelper.h"
#include "Serialization/MemoryReader.h"


FKFile::FKFile()
{
	m_BinaryArray.Empty();
	m_pMemoryReader = nullptr;
}

FKFile::~FKFile()
{
	Close();
}

bool FKFile::Open(FString FileName)
{

	if (!FFileHelper::LoadFileToArray(m_BinaryArray, *FileName))
		return false;

	m_pMemoryReader = new FMemoryReader(m_BinaryArray, true);
	m_pMemoryReader->Seek(0);

    return true;
}

void FKFile::Close()
{
	m_BinaryArray.Empty();

	if (m_pMemoryReader != nullptr)
	{
		m_pMemoryReader->Close();
		delete m_pMemoryReader;
		m_pMemoryReader = nullptr;
	}
		
}

int64 FKFile::Read(void* lpBuffer, unsigned long dwReadBytes)
{

	if (m_pMemoryReader == nullptr)
		return 0;

	unsigned long ulReadBytes = FMath::Min((int64)dwReadBytes, m_pMemoryReader->TotalSize());
	m_pMemoryReader->Serialize(lpBuffer, ulReadBytes);

	if ( !m_pMemoryReader->IsError() )
		m_pMemoryReader->Seek(ulReadBytes);
	else
		return 0;
	
	return ulReadBytes;
}

int64 FKFile::Seek(int64 nPos)
{
	if (m_pMemoryReader == nullptr)
		return SEEK_ERROR;

	m_pMemoryReader->Seek(nPos);

	return m_pMemoryReader->Tell();
}

int64 FKFile::Size()
{
	if (m_pMemoryReader == nullptr)
		return 0;

	return m_pMemoryReader->TotalSize();
}
