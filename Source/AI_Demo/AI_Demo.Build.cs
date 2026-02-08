// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AI_Demo : ModuleRules
{
	public AI_Demo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AI_Demo",
			"AI_Demo/Variant_Platforming",
			"AI_Demo/Variant_Platforming/Animation",
			"AI_Demo/Variant_Combat",
			"AI_Demo/Variant_Combat/AI",
			"AI_Demo/Variant_Combat/Animation",
			"AI_Demo/Variant_Combat/Gameplay",
			"AI_Demo/Variant_Combat/Interfaces",
			"AI_Demo/Variant_Combat/UI",
			"AI_Demo/Variant_SideScrolling",
			"AI_Demo/Variant_SideScrolling/AI",
			"AI_Demo/Variant_SideScrolling/Gameplay",
			"AI_Demo/Variant_SideScrolling/Interfaces",
			"AI_Demo/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
