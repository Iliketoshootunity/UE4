// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template<class T>
class SSingleton 
{
public:
	explicit SSingleton() {};
	virtual ~SSingleton() {};

	SSingleton(SSingleton const&) = delete;
	SSingleton& operator=(SSingleton const&) = delete;

public:
	static T* Get()
	{
		static T m_Singleton;
		return &m_Singleton;
	}
};
