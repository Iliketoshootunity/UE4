// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FileHelper.h"

/**
 * 
 */
class OVERLORD_API FCsvFile
{
private:
    UDataTable * pDataTable;
public:
    FCsvFile();
	~FCsvFile();

public:
    template<typename T>
    bool		Load(FString strFileName)
    {
        if (strFileName.IsEmpty())	
            return false;

        FString FileContent(TEXT(""));

        // 读取json文件
        if (!FFileHelper::LoadFileToString(FileContent, *strFileName))
            return false;

        if (pDataTable == NULL)
        {
            pDataTable = NewObject<UDataTable>();
            pDataTable->AddToRoot(); // 防止gc回收
        }

        pDataTable->RowStruct = T::StaticStruct();

        TArray<FString> ErrorData = pDataTable->CreateTableFromCSVString(FileContent);
        return ErrorData.Num() <= 0 ? true : false;
    }

    template<typename T>
    bool        Read(TArray< T*> &OutRowArray)
    {
        pDataTable->GetAllRows(TEXT("FCsvFile:Read"), OutRowArray);
        return true;
    }

    void        Close();
};
