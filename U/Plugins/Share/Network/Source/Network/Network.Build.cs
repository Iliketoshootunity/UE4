// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class Network : ModuleRules
{

    public Network(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
                
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
               
            }
			);


        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
				"Net",
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        if(Target.Type != TargetType.Program)
        {
            PrivateDependencyModuleNames.Add("Engine");
        }

        PublicDefinitions.Add("WITH_UNREAL4=1");
        //System.Console.WriteLine("#### Set Shipping Libuv Libraries ####:" + NetLibPath);
    }
}
