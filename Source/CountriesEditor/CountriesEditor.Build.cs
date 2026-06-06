using UnrealBuildTool;

public class CountriesEditor : ModuleRules
{
    public CountriesEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "Countries",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "HTTP",
                "DeveloperSettings",
                "EditorSubsystem",
                "ToolMenus"
            }
        );
    }
}