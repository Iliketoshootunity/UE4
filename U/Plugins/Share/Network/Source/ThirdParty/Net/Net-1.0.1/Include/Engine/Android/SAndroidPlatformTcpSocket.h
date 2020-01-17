#pragma once
#include "Engine/GenericPlatform/SGenericPlatformSocket.h"


struct SAndroidPlatformTcpSocket : public SGenericPlatformSocket
{
public:
    static  bool           Startup();
    static int32           Shutdown();
    static int32   		   Connect(SOCKET s, const char* ip, unsigned short port);
    static int32           SetSockOpt(SOCKET s, int32 level, int32 optname, const char* optval, SOCKLEN optlen);
	static int32           GetSockOpt(SOCKET s, int32 level, int32 optname, char* optval, SOCKLEN* optlen);
    static bool            SetNonBlocking(SOCKET s, bool bIsNonBlocking = true);
    static SOCKET          Accept(SOCKET s, sockaddr_in* addr = NULL, int* addrlen = NULL);
    static int64		   Recv(SOCKET s, char* buf, int64 len, int32 flags);
    static int64		   Send(SOCKET s, const char* buf, int64 len, int32 flags);
    static int32           Close(SOCKET s);
};

typedef SAndroidPlatformTcpSocket STcpSocket;