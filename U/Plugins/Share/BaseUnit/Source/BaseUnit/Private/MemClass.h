#pragma once
#include "CoreMinimal.h"

class BASEUNIT_API FMemClass
{
private:
	void*				m_lpMemPtr;
	unsigned long		m_lpMemLen;
public:
	FMemClass();
	~FMemClass();
	void*				Alloc(unsigned long dwSize);
	void				Free();
	void*				GetMemPtr() { return m_lpMemPtr; };
	unsigned long		GetMemLen() { return m_lpMemLen; };
};
