// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;


public class Protobuf : ModuleRules
{
    public Protobuf(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        bEnableShadowVariableWarnings = false;
        bEnableUndefinedIdentifierWarnings = false;
        bEnableExceptions = true;

        PublicDefinitions.Add("HAVE_PTHREAD");
        PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI");
        PublicDefinitions.Add("PROTOBUF_DISABLE_LITTLE_ENDIAN_OPT_FOR_TEST");

        if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            PublicDefinitions.Add("WINDOWS_IGNORE_PACKING_MISMATCH");
        }

    }
}
