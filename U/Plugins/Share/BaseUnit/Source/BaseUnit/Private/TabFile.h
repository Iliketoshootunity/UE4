#pragma once


#include "CoreMinimal.h"
#include "KList.h"
#include "MemClass.h"


//---------------------------------------------------------------------------
typedef struct tabOff
{
	unsigned long		dwOffset;
	unsigned long		dwLength;
} TABOFFSET;


class BASEUNIT_API FTabFile
{
private:
	int			m_Width;
	int			m_Height;
	FMemClass		m_Memory;
	FMemClass		m_OffsetTable;
private:
	 void		CreateTabOffset();
	 bool		GetFileValue(int nRow, int nColumn, char* lpRString, unsigned long dwSize);
	 int		Str2Col(char* szColumn);
public:
	FTabFile();
	~FTabFile();
	 bool		Load(FString strFileName);
	 bool		Save(FString FileName){return false;}; 
	 int		FindRow(char* szRow);
	 int		FindColumn(char* szColumn);
	 int		GetWidth() { return m_Width;};
	 int		GetHeight() { return m_Height;};
	 bool		GetString(int nRow, char* szColumn, char* lpDefault, char* lpRString, unsigned long dwSize, bool bColumnLab = true);
	 bool		GetString(int nRow, int nColumn, char* lpDefault, char* lpRString, unsigned long dwSize);
	 bool		GetString(char* szRow, char* szColumn, char* lpDefault, char* lpRString, unsigned long dwSize);
	 bool		GetInteger(int nRow, char* szColumn, int nDefault, int *pnValue, bool bColumnLab = true);
	 bool		GetInteger(int nRow, int nColumn, int nDefault, int *pnValue);
	 bool		GetInteger(char* szRow, char* szColumn, int nDefault, int *pnValue);
	 bool		GetFloat(int nRow, char* szColumn, float fDefault, float *pfValue, bool bColumnLab = true);
	 bool		GetFloat(int nRow, int nColumn, float fDefault, float *pfValue);
	 bool		GetFloat(char* szRow, char* szColumn, float fDefault, float *pfValue);
	 void		Clear();
	
};
//---------------------------------------------------------------------------