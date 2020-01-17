#pragma once
#include "Engine/HAL/SPlatform.h"

#define NET_LENGTH_TYPE uint32
#define NET_ID_TYPE uint32

#define NET_LENGTH_TYPE_BYTE sizeof(NET_LENGTH_TYPE)
#define NET_ID_TYPE_BYTE sizeof(NET_ID_TYPE)

#pragma	pack(push, 1)
//用于解析包头
typedef struct __NET_PROTOCOL_HEAD
{
	NET_LENGTH_TYPE PackLength;					//0-2147483647, 4字节
	NET_ID_TYPE ProtocolID;					//0-2147483647 ，4字节
} FNetHead;
#pragma	pack(pop)

