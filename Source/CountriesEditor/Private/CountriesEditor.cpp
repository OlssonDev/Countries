#include "CountriesEditor.h"

#include "CTCountriesEditorSubsystem.h"
#include "Engine/Texture2D.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"

#define LOCTEXT_NAMESPACE "FCountriesEditorModule"

DEFINE_LOG_CATEGORY(LogCountriesEditor);

void FCountriesEditorModule::StartupModule()
{
	GenerateToolsMenu();
}

void FCountriesEditorModule::ShutdownModule()
{
}

void FCountriesEditorModule::GenerateToolsMenu()
{
	FText ButtonLabel = LOCTEXT("GenerateCountriesButtonLabel", "Generate Countries Data Assets");
	FText ButtonTooltip = LOCTEXT("GenerateCountriesButtonTooltip", "Fetches country data from the REST Countries API and generates data assets for each country.");
	
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
	FToolMenuSection& Section = Menu->AddSection("Countries", LOCTEXT("CountriesMenuSection", "Countries"));
	Section.AddEntry(FToolMenuEntry::InitMenuEntry(
		"Generate Countries",
		TAttribute<FText>(ButtonLabel),
		TAttribute<FText>(ButtonTooltip),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "DeveloperTools.MenuIcon"),
		FUIAction(FExecuteAction::CreateStatic(&FCountriesEditorModule::OnGenerateCountries))
	));
}

void FCountriesEditorModule::OnGenerateCountries()
{
	if (!GEditor)
	{
		return;
	}
	
	UCTCountriesEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UCTCountriesEditorSubsystem>();
	EditorSubsystem->ExportCountriesAsDataAssets();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCountriesEditorModule, CountriesEditor)