// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class KursovaEditorTarget : TargetRules
{
	public KursovaEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		
		// New UE5 defaults
		bLegacyParentIncludePaths = false;
		CppStandard = CppStandardVersion.Default;
		WindowsPlatform.bStrictConformanceMode = true;
		bValidateFormatStrings = true;
		
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		
		ExtraModuleNames.AddRange( new string[] { "Kursova" } );
        
        // bOverrideBuildEnvironment = true;
        // AdditionalCompilerArguments = "-Wno-unused-but-set-variable";
	}
}
