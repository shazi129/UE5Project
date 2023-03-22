// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class InworldAIIntegration : ModuleRules
{
    private string ThirdPartyDirectory
    {
        get
        {
            return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/"));
        }
    }

    private string ThirdPartyLibrariesDirectory
    {
        get
        {
            if(Target.Platform == UnrealTargetPlatform.Win64)
            {
                return Path.Combine(ThirdPartyDirectory, "Libraries/Win64");
            }
            else if(Target.Platform == UnrealTargetPlatform.Mac)
            {
                return Path.Combine(ThirdPartyDirectory, "Libraries/Mac");
            }
            else if(Target.Platform == UnrealTargetPlatform.IOS)
            {
                return Path.Combine(ThirdPartyDirectory, "Libraries/iOS");
            }
            else
            {
                return Path.Combine(ThirdPartyDirectory, "Libraries/Unknown");
            }
        }
    }

    public InworldAIIntegration(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] 
            { 
                "Core", 
                "CoreUObject", 
                "Engine", 
                "InputCore",
                "InworldAIClient",
                "InworldAIPlatform",
                "InworldAINdk",
            });


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "ApplicationCore",
                "AudioCapture",
                "AudioMixer",
                "Projects",
            }
            );

        PublicIncludePaths.Add(Path.Combine(ThirdPartyDirectory, "Includes"));

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PrivateDefinitions.Add("INWORLD_WEB_RTC=1");

            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyLibrariesDirectory, "webrtc_aec_plugin.dll.lib"));

            RuntimeDependencies.Add(Path.Combine("$(BinaryOutputDir)", "webrtc_aec_plugin.dll"), Path.Combine(ThirdPartyLibrariesDirectory, "webrtc_aec_plugin.dll"));
        }

        PrivateDefinitions.Add("INWORLD_ONSCREEN_LOG=0");
        PrivateDefinitions.Add("INWORLD_ONSCREEN_LOG_ERROR=0");

        AddEngineThirdPartyPrivateStaticDependencies(Target, "zlib");
    }
}
