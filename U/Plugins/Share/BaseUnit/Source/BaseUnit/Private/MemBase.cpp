#include "MemBase.h"
#include "BaseUnitDef.h"
#include "BaseUnitLog.h"
#include "Node.h"
#include "KList.h"


//---------------------------------------------------------------------------
class FMemNode : public FNode
{
public:
	unsigned long	m_dwMemSize;//memory size
	unsigned long	m_dwMemSign;//Memory flag
};
//---------------------------------------------------------------------------
class FMemList : public FKList
{
public:
	~FMemList()
	{
		FMemNode* pNode = (FMemNode*)GetHead();
		while (pNode)
		{
			UE_LOG(Gamelog, Log, TEXT("KMemList::Leak Detected, Size = %d"), pNode->m_dwMemSize);
			pNode = (FMemNode*)pNode->GetNext();
		}
	};
	void ShowUsage()
	{
		FMemNode* pNode = (FMemNode*)GetHead();
		unsigned long dwMemSize = 0;
		while (pNode)
		{
			dwMemSize += pNode->m_dwMemSize;
			pNode = (FMemNode*)pNode->GetNext();
		}
		UE_LOG(Gamelog, Log, TEXT("Memory Usage Size = %d KB"), dwMemSize >> 10);
	}
};
static FMemList m_MemList;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Function:	g_MemAlloc
// Directions:	Allocate memory
// Parameter:	dwSize		Memory block size
// Return:	lpMem (lpMem = NULL Indicates allocation failed)
//---------------------------------------------------------------------------
void* g_MemAlloc(unsigned long dwSize)
{
	//	HANDLE hHeap = GetProcessHeap();
	unsigned char *  lpMem = NULL;
	unsigned long  dwHeapSize = dwSize + sizeof(FMemNode);

	//	lpMem = (PBYTE)HeapAlloc(hHeap, 0, dwHeapSize);
	lpMem = (unsigned char *)new char[dwHeapSize];
	if (NULL == lpMem)
	{
		UE_LOG(Gamelog, Error, TEXT("g_MemAlloc() Failed, Size = %d"), dwSize);
		return NULL;
	}

	FMemNode* pNode = (FMemNode*)lpMem;
	pNode->m_pPrev = NULL;
	pNode->m_pNext = NULL;
	pNode->m_dwMemSize = dwSize;
	pNode->m_dwMemSign = MEMSIGN;
	m_MemList.AddHead(pNode);

	return (lpMem + sizeof(FMemNode));
	//	return 0;
}
//---------------------------------------------------------------------------
// Function:	g_MemFree
// Directions:	Free memory
// Parameter:	lpMem		The memory pointer to release
// Return:	void
//---------------------------------------------------------------------------
void g_MemFree(unsigned char * lpMem)
{
	//	HANDLE hHeap = GetProcessHeap();
	if (lpMem == NULL)
		return;
	lpMem = (unsigned char *)lpMem - sizeof(FMemNode);
	FMemNode* pNode = (FMemNode *)lpMem;
	if (pNode->m_dwMemSign != MEMSIGN)
	{
		UE_LOG(Gamelog, Error, TEXT("g_MemAlloc() Failed, Size = %d"), pNode->m_dwMemSize);
		return;
	}
	pNode->Remove();
	//	HeapFree(hHeap, 0, lpMem);
	delete[] lpMem;
}
//---------------------------------------------------------------------------
// Function:	MemoryCopy
// Directions:	Memory copy
// Parameter:	lpDest	:	Target memory block
//			lpSrc	:	Source memory block
//			dwLen	:	Copy length
// Return:	void
//---------------------------------------------------------------------------
void g_MemCopy(void* lpDest, void* lpSrc, unsigned long dwLen)
{
	memcpy(lpDest, lpSrc, dwLen);
}
//---------------------------------------------------------------------------
// Function:	MemoryCopyMmx
// Directions:	Memory copy
// Parameter:	lpDest	:	Target memory block
//			lpSrc	:	Source memory block
//			dwLen	:	Copy length
// Return:	void
//---------------------------------------------------------------------------
void g_MemCopyMmx(void* lpDest, void* lpSrc, unsigned long dwLen)
{
	memcpy(lpDest, lpSrc, dwLen);
}
//---------------------------------------------------------------------------
// Function:	MemoryComp
// Directions:	Memory comparison
// Parameter:	lpDest	:	Memory block1
//			lpSrc	:	Memory block 2
//			dwLen	:	Compare length
// Return:	TRUE	:	The Same
//			FALSE	:	different	
//---------------------------------------------------------------------------
bool g_MemComp(void* lpDest, void* lpSrc, unsigned long dwLen)
{
	return (0 == memcmp(lpDest, lpSrc, dwLen));
}
//---------------------------------------------------------------------------
// Function:	MemoryZero
// Directions:	Memory clear
// Parameter:	lpDest	:	Memory address
//			dwLen	:	Memory Lenght
// Return:	void
//---------------------------------------------------------------------------
void g_MemZero(void* lpDest, unsigned long dwLen)
{
	memset(lpDest, 0, dwLen);
}


bool    g_StrCmp(const char* lpDest, const char* lpSrc)
{
	int nLen1, nLen2;

	nLen1 = strlen(lpDest);
	nLen2 = strlen(lpSrc);
	if (nLen1 != nLen2)
		return false;
	return g_MemComp((void*)lpDest, (void*)lpSrc, nLen1);
}

