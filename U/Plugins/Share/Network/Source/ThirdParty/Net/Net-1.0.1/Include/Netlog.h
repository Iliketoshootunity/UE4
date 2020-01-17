#pragma once
#include "Engine/HAL/SPlatform.h"

#ifndef SERVER_LOG 
#define SERVER_LOG 0
#endif

#if !WITH_UNREAL4 && SERVER_LOG
#include "lo_logs.hpp"
#endif

#ifndef NET_LOG_LENGTH
#define NET_LOG_LENGTH 512
#endif

typedef void (*NET_LOG)(const char* str);

class FNetLog
{	
public:
	FNetLog();
	~FNetLog();

	static void Init(NET_LOG inlogfun);
	
private:
	static NET_LOG m_logfun;

	static void LogString(char* log, int32 logsize, const char* fmt, ...);
public:

	template<typename... Types>
	static void Log(const char* fmt, Types... Args)
	{
#if !WITH_UNREAL4 && SERVER_LOG
		LoLogs::WriteLog(fmt, Args...);
#else
		if (!m_logfun)
			return;

		char buf[NET_LOG_LENGTH];
		LogString(buf, NET_LOG_LENGTH, fmt, Args...);

		m_logfun(buf);
#endif
	}
};




