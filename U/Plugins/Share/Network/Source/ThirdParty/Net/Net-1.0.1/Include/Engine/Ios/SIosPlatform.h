#pragma once

#include <errno.h>
#include <float.h>
#include <pthread.h>	
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <stdint.h>
#include <signal.h>
#include <net/if.h>
#include <iconv.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/resource.h> 
#include <algorithm>
#include <sys/poll.h>
#include <malloc/malloc.h>

#include "Engine/GenericPlatform/SGenericPlatform.h"

#if !WITH_UNREAL4

/**
* Linux specific types
**/
struct SIosPlatformTypes : public SGenericPlatformTypes
{
	typedef size_t				SIZE_T;
	typedef decltype(NULL)		TYPE_OF_NULL;
	typedef char16_t			WIDECHAR;
	typedef WIDECHAR			TCHAR;

};

typedef SIosPlatformTypes SPlatformTypes;

#endif


