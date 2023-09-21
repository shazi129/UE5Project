// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5Project : ModuleRules
{
	public UE5Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
            "UMG",
            "ExUMG",
			"InputCore",
			"GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "ApplicationCore",
            "ModularGameplay",
            "GameFeatures",
            "Projects",
            "SmartObjectsModule",

            //TestMass
            "MassEntity",
            "MassSpawner",
            "MassCommon",
            "MassRepresentation",
            "MassLOD",

            "StructUtils"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        AddDefaultIncludePaths();

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}

    private void AddDefaultIncludePaths()
    {
        // Add the module's parent directory to the public include paths, so other modules may include headers from it explicitly.
        //PublicIncludePaths.Add(ModuleDirectory.ParentDirectory);

        // Add the base directory to the legacy include paths.
        PublicIncludePaths.Add(ModuleDirectory);

        /*
        // Add the 'classes' directory, if it exists
        DirectoryReference ClassesDirectory = DirectoryReference.Combine(ModuleDirectory, "Classes");
        if (DirectoryLookupCache.DirectoryExists(ClassesDirectory))
        {
            PublicIncludePaths.Add(ClassesDirectory);
        }

        // Add all the public directories
        DirectoryReference PublicDirectory = DirectoryReference.Combine(ModuleDirectory, "Public");
        if (DirectoryLookupCache.DirectoryExists(PublicDirectory))
        {
            PublicIncludePaths.Add(PublicDirectory);

            ReadOnlyHashSet<string> ExcludeNames = UEBuildPlatform.GetBuildPlatform(Rules.Target.Platform).GetExcludedFolderNames();
            EnumerateLegacyIncludePaths(DirectoryItem.GetItemByDirectoryReference(PublicDirectory), ExcludeNames, LegacyPublicIncludePaths);
        }

        // Add the base private directory for this module
        DirectoryReference PrivateDirectory = DirectoryReference.Combine(ModuleDirectory, "Private");
        if (DirectoryLookupCache.DirectoryExists(PrivateDirectory))
        {
            PrivateIncludePaths.Add(PrivateDirectory);
        }
        */
    }
}
