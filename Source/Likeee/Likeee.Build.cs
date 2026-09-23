// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Likeee : ModuleRules
{
	public Likeee(ReadOnlyTargetRules Target) : base(Target)
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
			"Likeee",
			"Likeee/Variant_Platforming",
			"Likeee/Variant_Platforming/Animation",
			"Likeee/Variant_Combat",
			"Likeee/Variant_Combat/AI",
			"Likeee/Variant_Combat/Animation",
			"Likeee/Variant_Combat/Gameplay",
			"Likeee/Variant_Combat/Interfaces",
			"Likeee/Variant_Combat/UI",
			"Likeee/Variant_SideScrolling",
			"Likeee/Variant_SideScrolling/AI",
			"Likeee/Variant_SideScrolling/Gameplay",
			"Likeee/Variant_SideScrolling/Interfaces",
			"Likeee/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
