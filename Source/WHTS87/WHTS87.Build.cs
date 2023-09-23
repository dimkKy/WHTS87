// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WHTS87 : ModuleRules
{
	public WHTS87(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;

        if (Target.bBuildEditor)
        {
            bUseUnity = false;
        }

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "MoviePlayer", "CinematicCamera"/*, "Slate", "SlateCore"*/ });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
