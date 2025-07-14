// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Physics : ModuleRules
{
	public Physics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Physics", "GeometryCollectionEngine" });

        PrivateIncludePaths.Add("Physics");
    }
}
