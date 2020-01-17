// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSingleton.h"

/**
 * 
 */
class TABLEMODULE_API TableManager : public SSingleton<TableManager>
{
public:
    TableManager();
	~TableManager();

public:
    void                    Load();
    void                    UnLoad();

    FORCEINLINE     bool    IsLoaded()
    {
        return m_bLoaded;
    }

private:
    bool                    m_bLoaded;
};
