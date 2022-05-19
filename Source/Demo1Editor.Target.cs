using UnrealBuildTool;

public class Demo1EditorTarget : TargetRules
{
	public Demo1EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Demo1");
	}
}
