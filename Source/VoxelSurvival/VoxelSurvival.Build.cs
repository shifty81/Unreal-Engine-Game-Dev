// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VoxelSurvival : ModuleRules
{
	public VoxelSurvival(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"Networking",
			"Sockets",
			"UMG",
			"Slate",
			"SlateCore",
			"ProceduralMeshComponent"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// For modding support
		DynamicallyLoadedModuleNames.AddRange(new string[] {
			"OnlineSubsystem"
		});
	}
}
