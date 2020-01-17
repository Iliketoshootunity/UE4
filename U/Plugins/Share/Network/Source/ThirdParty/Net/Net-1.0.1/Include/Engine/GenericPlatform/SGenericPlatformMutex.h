#pragma once
#include "Engine/HAL/SPlatform.h"

struct SGenericPlatformMutex
{
public:
    virtual bool trylock() = 0;
    virtual void lock() = 0;
    virtual  void unlock() = 0;
};