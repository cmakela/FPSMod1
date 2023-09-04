// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSMod1 : ModuleRules
{
	public FPSMod1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		// Added dependency for Navigation and UMG, Slate, SlateCore
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "UMG", "Slate", "SlateCore" });
	}
}
