#pragma once
#include "Engine/HAL/SPlatform.h"

struct SGenericPlatformThread
{
public:
	enum enum_status
	{
		new_created,
		running,
		stopped
	};

	enum_status status;

	virtual void* action() = 0;
};