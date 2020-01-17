#include "MemClass.h"
#include "BaseUnitDef.h"
#include "BaseUnitLog.h"
#include "MemBase.h"


FMemClass::FMemClass()
{
	m_lpMemPtr = NULL;
	m_lpMemLen = 0;
}

FMemClass::~FMemClass()
{
	Free();
}
//---------------------------------------------------------------------------
// Function:	Alloc
// Directions:	Allocate memory
// Parameter:	dwSize		Memory block size
// Return:	PVOID		Memory Block Point
//---------------------------------------------------------------------------
void* FMemClass::Alloc(unsigned long dwSize)
{
	// Do not redistribute any more
	if (m_lpMemLen == dwSize)
		return m_lpMemPtr;

	// Release the allocated memory
	if (m_lpMemPtr)
		Free();

	// Allocate memory
	m_lpMemPtr = g_MemAlloc(dwSize);
	if (m_lpMemPtr != NULL)
		m_lpMemLen = dwSize;
	return m_lpMemPtr;
}

//---------------------------------------------------------------------------
// Function:	Free
// Directions:	free Memory
// Parameter:	void
// Return:	void
//---------------------------------------------------------------------------
void FMemClass::Free()
{
	if (m_lpMemPtr)
		g_MemFree((unsigned char *)m_lpMemPtr);
	m_lpMemPtr = NULL;
	m_lpMemLen = 0;
}
