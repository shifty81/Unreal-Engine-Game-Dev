// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class VoxelSurvivalEditorTarget : TargetRules
{
	public VoxelSurvivalEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("VoxelSurvival");
		
		// Use the latest installed MSVC toolchain (v143 - VS 2022)
		// Compatible with MSVC 14.30+ (including 14.44.35207)
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			WindowsPlatform.Compiler = WindowsCompiler.Default;
		}
	}
}
