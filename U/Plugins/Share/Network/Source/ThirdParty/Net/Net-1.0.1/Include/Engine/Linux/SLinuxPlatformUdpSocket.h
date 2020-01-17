#pragma once
#include "Engine/GenericPlatform/SGenericPlatformSocket.h"


struct SLinuxPlatformUdpSocket : public SGenericPlatformSocket
{
public:
    static struct sockaddr_in	   m_SrvAddr;
	static struct sockaddr_in*	   GetSockAddr()
	{
		return &m_SrvAddr;
	}
public:
    static  bool           Startup();
    static int32           Shutdown();
    static SSIZE_T 		   Connect(SOCKET s, const char* ip, unsigned short port);
    static int32           SetSockOpt(SOCKET s, int32 level, int32 optname, const char* optval, int32 optlen);
    static bool            SetNonBlocking(SOCKET s, bool bIsNonBlocking = true);
    static int64           Accept(SOCKET s, sockaddr_in* addr = NULL, SSIZE_T* addrlen = NULL);
    static int64		   Recv(SOCKET s, char* buf, int len, int flags);
    static int64		   Send(SOCKET s, const char* buf, int len, int flags);
    static int32           Close(SOCKET s);
};

typedef SLinuxPlatformUdpSocket SUdpSocket;
