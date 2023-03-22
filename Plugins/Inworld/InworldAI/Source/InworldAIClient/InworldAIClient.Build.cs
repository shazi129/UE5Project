// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class InworldAIClient : ModuleRules
{
   
    public InworldAIClient(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] 
            { 
                "Core", 
                "CoreUObject", 
                "Engine", 
                "InputCore",
                "InworldAINdk",
            });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Sockets",
                "OpenSSL",
            }
            );
    }
}
