#pragma once
#include "Engine/HAL/SPlatform.h"

#define RB_SPACE	( 8 )

class SRingBuffer
{

public:
    SRingBuffer();
    ~SRingBuffer();

    int32 Init(char* StartPtr, uint32 uBufLen, uint32 uMaxPack);

    int32 Reinit();

    int32 SndPack(const void* pData, uint32 datasize);

    const void* RcvPack(uint32& datasize);

private:

    /*
    *	写入数据,返回右指针
    */
    char* PutData(char* leftptr, char* rightptr, char* pData, uint32 datasize);

    uint32 GetDataLen(char* leftptr, char* rightptr);

    void Clear();

private:

    char* m_startptr;
    char* m_leftptr;
    char* m_rightptr;
    char* m_tmpptr;
    char* m_nextleft;
    /*
    *	var
    */

    uint32	m_uBufLen;
    uint32	m_uMaxPack;
};
