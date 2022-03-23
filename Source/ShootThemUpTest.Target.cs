

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUpTestTarget : TargetRules
{
	public ShootThemUpTestTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ShootThemUpTest" } );
	}
}
