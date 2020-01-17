#include "TabFile.h"
#include "KFile.h"
#include "BaseUnitDef.h"
#include "BaseUnitLog.h"
#include "KPakFile.h"
#include "MemBase.h"


//---------------------------------------------------------------------------
// Function:	FTabFile
// Directions:	Purchase function
// Parameter:	void
// Return:	void
//---------------------------------------------------------------------------
FTabFile::FTabFile()
{
	m_Width = 0;
	m_Height = 0;
}
//---------------------------------------------------------------------------
// Function:	~FTabFile
// Directions:	Analysis function
// Parameter:	void
// Return:	void
//---------------------------------------------------------------------------
FTabFile::~FTabFile()
{
	Clear();
}
//---------------------------------------------------------------------------
// Function:	Load
// Directions:	Load a Tab file
// Parameter:	FileName	
// Return:	true		Successed
//			false		Failed
//---------------------------------------------------------------------------
bool FTabFile::Load(FString strFileName)
{
	FKPakFile	File;
	unsigned long		dwSize;
	void*		Buffer;

	// check file name
	if (strFileName.IsEmpty())
		return false;

	if (!File.Open(strFileName))
	{
		UE_LOG(Gamelog, Log, TEXT("Can't open tab file : %s"), *strFileName);
		return false;
	}

	dwSize = File.Size();

	Buffer = m_Memory.Alloc(dwSize);

	File.Read(Buffer, dwSize);
	
	File.Close();

	if (dwSize)
		CreateTabOffset();
	else
		return false;

	return true;
}

//---------------------------------------------------------------------------
// Function:	CreateTabOffset
// Directions:	Create an offset table for making a tab delimited file
// Parameter:	void
// Return:	void
//---------------------------------------------------------------------------
void FTabFile::CreateTabOffset()
{
	int		nWidth, nHeight, nOffset, nSize;
	unsigned char	*Buffer;
	TABOFFSET *TabBuffer;

	nWidth	= 1;
	nHeight	= 1;
	nOffset = 0;

	Buffer	= (unsigned char *)m_Memory.GetMemPtr();
	nSize	= m_Memory.GetMemLen();
	
	if (!Buffer || !nSize)
		return;

	// Read the first row to determine how many columns there are
	while (*Buffer != 0x0d && *Buffer != 0x0a)
	{
		if (*Buffer == 0x09)
		{
			nWidth++;
		}
		Buffer++;
		nOffset++;
	}
	if (*Buffer == 0x0d && *(Buffer + 1) == 0x0a)
	{
		Buffer += 2;	// 0x0a jump over	
		nOffset += 2;	// 0x0a jump over
	}
	else
	{
		Buffer += 1;	// 0x0a jump over		
		nOffset += 1;	// 0x0a jump over
	}
	while(nOffset < nSize)
	{
		while (*Buffer != 0x0d && *Buffer != 0x0a)
		{
			Buffer++;
			nOffset++;
			if (nOffset >= nSize)
				break;
		}
		nHeight++;
		if (*Buffer == 0x0d && *(Buffer + 1) == 0x0a)
		{
			Buffer += 2;	// 0x0a jump over	
			nOffset += 2;	// 0x0a jump over
		}
		else
		{
			Buffer += 1;	// 0x0a jump over	
			nOffset += 1;	// 0x0a jump over
		}
	}
	m_Width		= nWidth;
	m_Height	= nHeight;

	TabBuffer = (TABOFFSET *)m_OffsetTable.Alloc(m_Width * m_Height * sizeof (TABOFFSET));
	Buffer = (unsigned char *)m_Memory.GetMemPtr();

	nOffset = 0;
	int nLength;
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			TabBuffer->dwOffset = nOffset;	
			nLength = 0;
			while(*Buffer != 0x09 && *Buffer != 0x0d && *Buffer != 0x0a && nOffset < nSize)
			{
				Buffer++;
				nOffset++;
				nLength++;
			}
			Buffer++;	// 0x09 or 0x0d or 0x0a(linux) jump over
			nOffset++;
			TabBuffer->dwLength = nLength;
			TabBuffer++;
			if (*(Buffer - 1) == 0x0a || *(Buffer - 1) == 0x0d)	//	The bank is already over, although it may not have arrived nWidth //for linux modified [wxb 2003-7-29]
			{
				for (int k = j+1; k < nWidth; k++)
				{
					TabBuffer->dwOffset = nOffset;
					TabBuffer->dwLength = 0;
					TabBuffer++;					
				}
				break;
			}
		}

		//modified for linux [wxb 2003-7-29]
		if (*(Buffer - 1) == 0x0d && *Buffer == 0x0a)
		{
			Buffer++;				// 0x0a jump over	
			nOffset++;				// 0x0a jump over
		}
	}
}

//---------------------------------------------------------------------------
// Function:	Str2Column
// Directions:	Get the value of a row of a column string
// Parameter:	szColumn
// Return:	Columns
//---------------------------------------------------------------------------
int FTabFile::Str2Col(char* szColumn)
{
	
	int	nStrLen = strlen(szColumn);
	char	szTemp[4];

	//strcpy(szTemp, szColumn);
	FCStringAnsi::Strcpy(szTemp, szColumn);

	if (nStrLen == 1)
	{
		return (szTemp[0] - 'A');
	}
	return ((szTemp[0] - 'A' + 1) * 26 + szTemp[1] - 'A') + 1;
}

//---------------------------------------------------------------------------
// Function:	GetString
// Directions:	Get the value of a row of a column string
// Parameter:	nRow			Row
//			nColomn			Columns
//			lpDefault		Default value
//			lpRString		return value
//			dwSize			Returns the maximum length of a string
// Return:	whether succeed
//---------------------------------------------------------------------------
bool FTabFile::GetString(int nRow, char* szColumn, char* lpDefault, char* lpRString, unsigned long dwSize, bool bColumnLab)
{
	int nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);

	//Row starts from the second row
	if (GetFileValue(nRow - 1, nColumn - 1, lpRString, dwSize))
		return true;

	//strncpy(lpRString, lpDefault, dwSize);
	FCStringAnsi::Strncpy(lpRString, lpDefault, dwSize);
	return false;
}
//---------------------------------------------------------------------------
// Function:	GetString
// Directions:	Get the value of a row of a column string
// Parameter:	szRow		Line (keyword)
//			szColomn		Column (keyword)
//			lpDefault		Default value
//			lpRString		return value
//			dwSize			Returns the maximum length of a string
// Return:	whether succeed
//---------------------------------------------------------------------------
bool FTabFile::GetString(char* szRow, char* szColumn, char* lpDefault, char* lpRString, unsigned long dwSize)
{
	int nRow, nColumn;

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);
	if (GetFileValue(nRow - 1, nColumn - 1, lpRString, dwSize))
		return true;

	//strncpy(lpRString, lpDefault, dwSize);
	FCStringAnsi::Strncpy(lpRString, lpDefault, dwSize);
	return false;
}
//---------------------------------------------------------------------------
// Function:	GetString
// Directions:	Get the value of a row of a column string
// Parameter:	nRow		Line (keyword)
//			nColomn			Column (keyword)
//			lpDefault		Default value
//			lpRString		return value
//			dwSize			Returns the maximum length of a string
// Return:	whether succeed
//---------------------------------------------------------------------------
bool FTabFile::GetString(int nRow, int nColumn, char* lpDefault, char* lpRString, unsigned long dwSize)
{
	if (GetFileValue(nRow - 1, nColumn - 1,  lpRString, dwSize))
		return true;

	//strncpy(lpRString, lpDefault, dwSize);
	FCStringAnsi::Strncpy(lpRString, lpDefault, dwSize);
	return false;
}

bool FTabFile::GetInteger(int nRow, char* szColumn, int nDefault, int *pnValue, bool bColumnLab)
{
	char    	Buffer[32];
	int		nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	if (GetFileValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		return true;
	}
	else
	{
		*pnValue = nDefault;
		return false;
	}
}

bool FTabFile::GetInteger(char* szRow, char* szColumn, int nDefault, int *pnValue)
{
	int		nRow, nColumn;
	char		Buffer[32];

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);
	if (GetFileValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		return true;
	}
	else
	{
		*pnValue = nDefault;
		return false;
	}
}

bool FTabFile::GetInteger(int nRow, int nColumn, int nDefault, int *pnValue)
{
	char	Buffer[32];

	if (GetFileValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		return true;
	}
	else
	{
		*pnValue = nDefault;
		return true;
	}
}

bool FTabFile::GetFloat(int nRow, char* szColumn, float fDefault, float *pfValue, bool bColumnLab)
{
	char		Buffer[32];
	int		nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	if (GetFileValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pfValue = (float)atof(Buffer);
		return true;
	}
	else
	{
		*pfValue = fDefault;
		return false;
	}
}

bool FTabFile::GetFloat(char* szRow, char* szColumn, float fDefault, float *pfValue)
{
	int		nRow, nColumn;
	char		Buffer[32];

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);
	if (GetFileValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pfValue = (float)atof(Buffer);
		return true;
	}
	else
	{
		*pfValue = fDefault;
		return false;
	}
}

bool FTabFile::GetFloat(int nRow, int nColumn, float fDefault, float *pfValue)
{
	char	Buffer[32];
	
	if (GetFileValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pfValue = (float)atof(Buffer);
		return true;
	}
	else
	{
		*pfValue = fDefault;
		return false;
	}
}

bool FTabFile::GetFileValue(int nRow, int nColumn, char* lpRString, unsigned long dwSize)
{
	if (nRow >= m_Height || nColumn >= m_Width || nRow < 0 || nColumn < 0)
		return false;

	TABOFFSET	*TempOffset;
	char*		Buffer;

	Buffer = (char*)m_Memory.GetMemPtr();
	TempOffset = (TABOFFSET *)m_OffsetTable.GetMemPtr();
	TempOffset += nRow * m_Width + nColumn;

	memset(lpRString, 0, dwSize);
	Buffer += TempOffset->dwOffset;
	if (TempOffset->dwLength == 0)
	{
		return false;
	}

	if (dwSize > TempOffset->dwLength)
	{
		memcpy(lpRString, Buffer, TempOffset->dwLength);
		lpRString[TempOffset->dwLength] = 0;
	}
	else
	{
		memcpy(lpRString, Buffer, dwSize);
		lpRString[dwSize] = 0;
	}

	return true;
}

void FTabFile::Clear()
{
	m_Width = 0;
	m_Height = 0;
	m_Memory.Free();
	m_OffsetTable.Free();

}

int FTabFile::FindRow(char* szRow)
{
	char	szTemp[128];

	for (int i = 0; i < m_Height; i++)	
	{
		bool bFindResult = GetFileValue(i, 0, szTemp, sizeof(szTemp));
		if (!bFindResult)
			continue;

		if( g_StrCmp(szTemp, szRow) )
			return i + 1; 
	}
	return -1;
}

int FTabFile::FindColumn(char* szColumn)
{
	char	szTemp[128];
	bool bFindResult = false;
	for (int i = 0; i < m_Width; i++)	
	{
		bFindResult = GetFileValue(0, i, szTemp, sizeof(szTemp));
		if(!bFindResult)
			continue;

		if (g_StrCmp(szTemp, szColumn))
			return i + 1;
	}
	return -1;
}
