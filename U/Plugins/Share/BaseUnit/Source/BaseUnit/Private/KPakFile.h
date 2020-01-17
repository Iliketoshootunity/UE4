#pragma once
//---------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "KFile.h"


class  BASEUNIT_API FKPakFile
{
public:
	 FKPakFile();
	 ~FKPakFile();
	 bool		Open(FString pszFileName);
	 void		Close();
	 unsigned long		Read(void* pBuffer, uint32 uSize);
	 unsigned long		Seek(int64 nOffset);
	 unsigned long		Size();
private:
	FKFile				m_File;			
};
