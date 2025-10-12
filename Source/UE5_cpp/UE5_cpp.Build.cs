// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5_cpp : ModuleRules
{
	public UE5_cpp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
