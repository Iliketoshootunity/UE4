#pragma once
#include "CoreMinimal.h"

void*	g_MemAlloc(unsigned long dwSize);
void	g_MemFree(unsigned char * lpMem);
void	g_MemCopy(void * lpDest, void * lpSrc, unsigned long dwLen);
void	g_MemCopyMmx(void * lpDest, void * lpSrc, unsigned long dwLen);
bool 	g_MemComp(void * lpDest, void * lpSrc, unsigned long dwLen);
void	g_MemZero(void * lpDest, unsigned long dwLen);
bool    g_StrCmp(const char* lpDest,const char* lpSrc);

