#pragma once
//
#include "NetProtocol.h"
#include "NetworkInterface.h"

#ifndef NET_INVALID_VALUE
#define NET_INVALID_VALUE			( -1 )
#endif

#ifndef NET_SUCCESS_VALUE
#define NET_SUCCESS_VALUE			( 0 )
#endif


#ifndef NET_B
#define NET_B						( 1 )
#endif

#ifndef NET_KB
#define NET_KB						( 1024 )
#endif

#ifndef NET_MB
#define NET_MB						( 1024 * 1024 )
#endif

#ifndef RB_SPACE
#define RB_SPACE				( 8 )
#endif

#ifndef RB_MAXPACK
#define RB_MAXPACK				( NET_KB * 4  )
#endif

#ifndef MAX_SENDBUF
#define MAX_SENDBUF				( NET_MB * 1 )
#endif

#ifndef MAX_RECVBUF
#define MAX_RECVBUF				( NET_MB * 1 )
#endif

#ifndef MAX_PAYPACK
#define MAX_PAYPACK				( NET_KB )
#endif


#define MAKEULLONG(a, b)     ((uint64)(((uint16)((a) & 0xffffffffffff)) | ((uint64)((uint32)((b) & 0xffffffffffff))) << 16))
#define LOSHORT(w)           ((uint16)((uint64)(w) & 0xffffffffffff))
#define HIULONG(w)           ((uint32)((uint64)(w) >> 16))

typedef enum _NetStatus
{
	Disconnected = -1,	//未连接
	Connected,			//已连接
} NetStatus;

//扰码处理
const uint32 ENCODE_KEY[] = {
	//0
	0xE1909063,
	0xC630DAF7,
	0x1DE0B27A,
	0x37C0154E,
	//4
	0x5CB061E2,
	0x71B0BA0B,
	0x99E006DC,
	0xB000DD03,
	//8
	0x06E06DFB,
	0xDAE0595C,
	0x13F08D64,
	0x7FE0C976,
	//12
	0xD54023EF,
	0xCC805BD5,
	0xECE00491,
	0x3850FA4C,
};

//开关
//#define ENCODE_ON

#define ENCODE_RAND(n)					(((n) & 0xF0000) >> 16)

//	加密包头长度信息
#ifdef ENCODE_ON
#define ENCODE_PACK_SIZE(n)				do{													\
											(n) ^= ENCODE_KEY[ENCODE_RAND(n)];				\
										}while(0)


//	解密包头长度信息
#define DECODE_PACK_SIZE(n)				do{														\
											(n) ^= ENCODE_KEY[ENCODE_RAND(n)];					\
											(n) = ((n) & 0xFFFF) | (((n) & 0xFFF00000) >> 4);	\
										}while(0)
#else
#define ENCODE_PACK_SIZE(n)
#define DECODE_PACK_SIZE(n)	
#endif


//socket 是否为阻塞
#ifndef SET_NONBLOCK
#define SET_NONBLOCK 0
#endif

//是否校验大小端
#ifndef VERIFYENDIAN
#define VERIFYENDIAN 1
#endif

//服务器是否为大端
#ifndef SERVER_BIGENDIAN
#define SERVER_BIGENDIAN 1
#endif



