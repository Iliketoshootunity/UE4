// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TableModule.h"
#include "TableModuleLog.h"
#include "TableManager.h"

#define LOCTEXT_NAMESPACE "FTableModuleModule"

DEFINE_LOG_CATEGORY(LogTableModule);

void FTableModuleModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    TableManager::Get()->Load();
}

void FTableModuleModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
    TableManager::Get()->UnLoad();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTableModuleModule, TableModule)