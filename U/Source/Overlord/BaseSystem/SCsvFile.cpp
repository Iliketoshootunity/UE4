// Fill out your copyright notice in the Description page of Project Settings.

#include "SCsvFile.h"
#include "Misc/FileHelper.h"

FCsvFile::FCsvFile()
{
    pDataTable = NULL;
}

FCsvFile::~FCsvFile()
{
    Close();
}

void FCsvFile::Close()
{
    if (!pDataTable || !pDataTable->IsValidLowLevel())
        return;

    pDataTable->RemoveFromRoot();
    pDataTable->ConditionalBeginDestroy();
    pDataTable = NULL;
}
