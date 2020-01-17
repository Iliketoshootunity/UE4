#pragma once
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "CoreMinimal.h"
//#include "Serialization/MemoryReader.h"

class FMemoryReader;

class BASEUNIT_API FKFile
{
private:

	TArray<uint8>		m_BinaryArray;
	FMemoryReader*		m_pMemoryReader;

public:
	FKFile();
	~FKFile();
	bool		Open(FString FileName);
	void		Close();
	int64		Read(void* lpBuffer, unsigned long dwReadBytes);
	int64		Seek(int64 nPos);
	int64		Size();
};

