#pragma once

#include "CoreMinimal.h"

class BASEUNIT_API FRingBuffer
{
public:
	FRingBuffer();
	virtual ~FRingBuffer();

	bool SndPack(const void* pData, uint32 datasize);
	bool SndPack(const void* pData1, const void* pData2, uint32 datasize1, uint32 datasize2);
	const void* RcvPack(uint32& datasize);

	bool Init(char* StartPtr, uint32 uBufLen, uint32 uMaxPack);
	bool Reinit();

private:
	char* PutData(char* leftptr, char* rightptr, char* pData, uint32 datasize);

	uint32 GetDataLen(char* leftptr, char* rightptr);

	void Clear();

private:
	char* m_startptr;
	char* m_leftptr;
	char* m_rightptr;
	char* m_tmpptr;
	char* m_nextleft;

	uint32	m_uBufLen;
	uint32	m_uMaxPack;
};