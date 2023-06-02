// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Room_VR_Test_1EditorTarget : TargetRules
{
	public Room_VR_Test_1EditorTarget( TargetInfo Target) : base(Target)
	{
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
        Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Room_VR_Test_1" } );
	}
}
