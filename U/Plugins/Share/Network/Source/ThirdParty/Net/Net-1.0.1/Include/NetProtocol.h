#pragma once
#include "Engine/HAL/SPlatform.h"

#define NET_LENGTH_TYPE uint32
#define NET_ID_TYPE uint32

#define NET_LENGTH_TYPE_BYTE sizeof(NET_LENGTH_TYPE)
#define NET_ID_TYPE_BYTE sizeof(NET_ID_TYPE)

#pragma	pack(push, 1)
//���ڽ�����ͷ
typedef struct __NET_PROTOCOL_HEAD
{
	NET_LENGTH_TYPE PackLength;					//0-2147483647, 4�ֽ�
	NET_ID_TYPE ProtocolID;					//0-2147483647 ��4�ֽ�
} FNetHead;
#pragma	pack(pop)

