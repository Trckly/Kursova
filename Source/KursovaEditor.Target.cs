// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class KursovaEditorTarget : TargetRules
{
	public KursovaEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Kursova" } );
        
        bOverrideBuildEnvironment = true;
        AdditionalCompilerArguments = "-Wno-unused-but-set-variable";
	}
}
