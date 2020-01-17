#pragma once
#include "Engine/HAL/SPlatform.h"

struct SGenericPlatformSocket
{
public:
    static SOCKET           Socket(int32 af, int32 type, int32 protocol);
    static int32            Bind(SOCKET s, const uint16 port);
    static int32            Bind(SOCKET s, sockaddr_in* addr, int32 addrlen);
    static int32            Listen(SOCKET s, int32 backlog);
    static int32            Select(int32 nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timeval * timeout);
    static int32            GetPeerName(SOCKET s, sockaddr* name, int32* namelen);
    static char*            Inet_ntoa(in_addr in);
    static unsigned         Inet_addr(const char* cp);
};