// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class Net : ModuleRules
{
    private string NetPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "Net-1.0.1/")); }
    }

    private string NetLibPath
    {
        get { return NetPath + "Lib/"; }
    }

    public Net(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        //System.Console.WriteLine("NetPath==============:" + NetPath);

        PublicSystemIncludePaths.Add(NetPath + "Include/");

        PublicDefinitions.Add("WITH_UNREAL4=1");

        string BuildConfig = "Release";
        if (Target.bDebugBuildsActuallyUseDebugCRT)
        {
            BuildConfig = "Debug";
        }

        if (Target.Platform == UnrealTargetPlatform.Win32)
        {

            PublicDefinitions.Add("WINDOWS_IGNORE_PACKING_MISMATCH");
            PublicAdditionalLibraries.Add(Path.Combine(NetLibPath, BuildConfig, "Win32", "Net.lib"));

        }
        else if (Target.Platform == UnrealTargetPlatform.Win64)
        {

            PublicAdditionalLibraries.Add(Path.Combine(NetLibPath, BuildConfig, "Win64", "Net.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicAdditionalLibraries.Add(Path.Combine(NetLibPath,  BuildConfig, "Ios", "libNet.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicLibraryPaths.Add(NetLibPath + BuildConfig + "/Android/armeabi-v7a/");
            PublicLibraryPaths.Add(NetLibPath + BuildConfig + "/Android/arm64-v8a/");
            PublicLibraryPaths.Add(NetLibPath + BuildConfig + "/Android/x86/");
            PublicLibraryPaths.Add(NetLibPath + BuildConfig + "/Android/x86_64/");

            PublicAdditionalLibraries.Add("Net");
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            PublicLibraryPaths.Add(NetLibPath + BuildConfig + "/Linux/");
            PublicAdditionalLibraries.Add("Net");
        }
    }
}
