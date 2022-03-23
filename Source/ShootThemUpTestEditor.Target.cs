

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUpTestEditorTarget : TargetRules
{
	public ShootThemUpTestEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ShootThemUpTest" } );

		bCompileChaos = true;
		bUseChaos = true;
	}
}
