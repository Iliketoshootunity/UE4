// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MessageModule.h"
#include "MessageListenerAgent.h"
#include "Message.h"

#define LOCTEXT_NAMESPACE "FMessageModuleModule"

void FMessageModuleModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FMessageModuleModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FMessageListenerAgent::ShutDown();
	FMessage::ShutDown();

}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMessageModuleModule, MessageModule)