using UnrealBuildTool;
using System.Collections.Generic;

public class Demo1Target : TargetRules
{
	public Demo1Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("Demo1");
	}
}
