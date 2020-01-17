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
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <functional>
#include <memory>
#include <linux/types.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/resource.h> 
#include <linux/errqueue.h>
#include <algorithm>
#include <linux/version.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include "Engine/GenericPlatform/SGenericPlatform.h"

#if !WITH_UNREAL4
/**
* Linux specific types
**/
struct SAndroidPlatformTypes : public SGenericPlatformTypes
{
	typedef char16_t					WIDECHAR;
	typedef WIDECHAR					TCHAR;
};

typedef SAndroidPlatformTypes SPlatformTypes;

#endif


