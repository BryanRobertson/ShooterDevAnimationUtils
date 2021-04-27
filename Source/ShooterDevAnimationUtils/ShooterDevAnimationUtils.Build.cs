// Copyright Bryan Robertson, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShooterDevAnimationUtils : ModuleRules
{
	public ShooterDevAnimationUtils(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;			
		
		PublicDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Core",
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"AnimationModifiers"
			}
		);
	}
}
