using UnrealBuildTool;

public class Demo1 : ModuleRules
{
	public Demo1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new [] {
            "Core", "CoreUObject", "Engine", "InputCore", "GameplayTasks", "NavigationSystem"
        });
	}
}
