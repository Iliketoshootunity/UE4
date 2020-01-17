#include "KPakFile.h"
#include "BaseUnitDef.h"
#include "BaseUnitLog.h"
#include "Misc/Paths.h"


//---------------------------------------------------------------------------
// Directions:	Purchase function
//---------------------------------------------------------------------------
FKPakFile::FKPakFile()
{

}

//---------------------------------------------------------------------------
// Directions:	Analysis function
//---------------------------------------------------------------------------
FKPakFile::~FKPakFile()
{
	Close();
}

//---------------------------------------------------------------------------
// Directions:	Open a file, first look for a separate file with the same name in the current directory
// Parameter:	FileName	
// Return:	TRUE		Successed
//			false		Failed
//---------------------------------------------------------------------------

bool FKPakFile::Open(FString pszFileName)
{
	if (pszFileName.IsEmpty())
		return false;

	bool bOk = false;
	Close();

	FString szPathFileName = FString::Format(TEXT("{0}{1}"), { FPaths::ProjectContentDir() ,pszFileName });

	FString szPackNameAddtion = FPaths::ConvertRelativePathToFull(szPathFileName);


	bOk = (m_File.Open(szPackNameAddtion) != false);
	
	return bOk;
}

//---------------------------------------------------------------------------
// Directions:	The length to read
// Parameter:	pBuffer		Buffer pointer
//			dwSize		The length to read
// Return:	The length of the byte read
//---------------------------------------------------------------------------
unsigned long FKPakFile::Read(void* pBuffer, uint32 uSize)
{
	uSize = m_File.Read(pBuffer, uSize);

	return uSize;
}

//---------------------------------------------------------------------------
// Directions:	File read pointer positioning
// Parameter:	lOffset			Offset
//			dwMethod		Positioning method
// Return:	File pointer
//---------------------------------------------------------------------------
unsigned long FKPakFile::Seek(int64 nOffset)
{
	nOffset = m_File.Seek(nOffset);
	return nOffset;
}

//---------------------------------------------------------------------------
// Directions:	Return file size
// Return:	The size of the file in bytes
//---------------------------------------------------------------------------
unsigned long FKPakFile::Size()
{
	unsigned long uSize;

	uSize = m_File.Size();
	return uSize;
}
//---------------------------------------------------------------------------
// Directions:	Close a file
//---------------------------------------------------------------------------
void FKPakFile::Close()
{
	m_File.Close();
}
