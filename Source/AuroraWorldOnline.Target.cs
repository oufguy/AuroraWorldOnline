using UnrealBuildTool;
using System.Collections.Generic;

public class AuroraWorldOnlineTarget : TargetRules
{
	public AuroraWorldOnlineTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("AuroraWorldOnline");
		RegisterModulesCreatedByRider();
	}
	
	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new string[] { "Aurora_Lib" });
	}
}
