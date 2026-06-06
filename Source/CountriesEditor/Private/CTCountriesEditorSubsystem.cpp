// Copyright (c) OlssonDev 2026. All Rights Reserved.

#include "CTCountriesEditorSubsystem.h"
#include "AssetToolsModule.h"
#include "CountriesEditor.h"
#include "CTCountriesDeveloperSettings.h"
#include "CTCountriesSubsystem.h"
#include "CTCountryPrimaryDataAsset.h"
#include "HttpModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Factories/DataAssetFactory.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Interfaces/IHttpResponse.h"
#include "UObject/SavePackage.h"
#include "Widgets/Notifications/SNotificationList.h"

void UCTCountriesEditorSubsystem::ExportCountriesAsDataAssets()
{
	if (IsExporting)
	{
		UE_LOG(LogCountriesEditor, Warning, TEXT("Already exporting countries; please wait until the current export is finished."));
		return;
	}
	
	IsExporting = true;
	StartNotification();
	
	UCTCountriesSubsystem* CountriesSubsystem = GEngine->GetEngineSubsystem<UCTCountriesSubsystem>();
	CountriesSubsystem->FetchAllCountries(FOnCountriesFetched::CreateUObject(this, &UCTCountriesEditorSubsystem::OnAllCountriesFetched));
}

void UCTCountriesEditorSubsystem::OnAllCountriesFetched(const TArray<FCTCountryInfo>& Countries, bool bSuccess)
{
	if (!bSuccess)
	{
		UE_LOG(LogCountriesEditor, Error, TEXT("Country fetch failed; nothing exported."));
		return;
	}

	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

	UDataAssetFactory* Factory = NewObject<UDataAssetFactory>();
	Factory->DataAssetClass = UCTCountryPrimaryDataAsset::StaticClass();

	JobQueue.Reset();
	CurrentJobIndex = 0;
	JobsInProgress = 0;
	PendingImages.Reset();
	
	CountryDataAssets.Reset(Countries.Num());
	
	const UCTCountriesDeveloperSettings* Settings = GetDefault<UCTCountriesDeveloperSettings>();
	const FString& BasePackagePath = Settings->BasePackagePath;
	const FString& FlagTexturePrefix = Settings->FlagTexturePrefix;
	const FString& CoatOfArmsTexturePrefix = Settings->CoatOfArmsTexturePrefix;

	for (const FCTCountryInfo& Country : Countries)
	{
		const FString SanitizedName = ObjectTools::SanitizeObjectName(Country.Name.Common);
		const FString AssetName = FString::Printf(TEXT("Country_%s"), *SanitizedName);
		const FString ObjectPath = FString::Printf(TEXT("%s/%s.%s"), *BasePackagePath, *AssetName, *AssetName);

		UObject* Asset = StaticLoadObject(UCTCountryPrimaryDataAsset::StaticClass(), nullptr, *ObjectPath);
		if (!IsValid(Asset))
		{
			Asset = AssetTools.CreateAsset(AssetName, BasePackagePath, UCTCountryPrimaryDataAsset::StaticClass(), Factory);
		}

		UCTCountryPrimaryDataAsset* CountryDataAsset = CastChecked<UCTCountryPrimaryDataAsset>(Asset);
		CountryDataAssets.Add(CountryDataAsset);
		
		CountryDataAsset->CountryInfo = Country;
		
		(void)CountryDataAsset->MarkPackageDirty();
		
		int32 ImageCount = 0;

		if (!Country.Flags.Png.IsEmpty() && CountryDataAsset->CoatOfArmsTexture.IsNull())
		{
			JobQueue.Add(FImageJob{
				Country.Flags.Png,
				FString::Printf(TEXT("%s/Flags/%s%s"), *BasePackagePath, *FlagTexturePrefix, *SanitizedName),
				FString::Printf(TEXT("%s%s"), *FlagTexturePrefix, *SanitizedName),
				CountryDataAsset,
				true});
			
			++ImageCount;
		}

		if (!Country.CoatOfArms.Png.IsEmpty() && CountryDataAsset->CoatOfArmsTexture.IsNull())
		{
			JobQueue.Add(FImageJob{
				Country.CoatOfArms.Png,
				FString::Printf(TEXT("%s/CoatOfArms/%s%s"), *BasePackagePath, *CoatOfArmsTexturePrefix, *SanitizedName),
				FString::Printf(TEXT("%s%s"), *CoatOfArmsTexturePrefix, *SanitizedName),
				CountryDataAsset,
				false});
			
			++ImageCount;
		}

		if (ImageCount > 0)
		{
			PendingImages.Add(CountryDataAsset, ImageCount);
		}
	}

	ProcessDownloads();
}

void UCTCountriesEditorSubsystem::ProcessDownloads()
{
	int32 MaxConcurrentDownloads = GetDefault<UCTCountriesDeveloperSettings>()->MaxConcurrentDownloads;
	
	while (JobsInProgress < MaxConcurrentDownloads && CurrentJobIndex < JobQueue.Num())
	{
		StartDownloadJob(JobQueue[CurrentJobIndex++]);
	}
	
	if (JobsInProgress == 0)
	{
		FinishExport();
	}
}

void UCTCountriesEditorSubsystem::StartDownloadJob(const FImageJob& Job)
{
	++JobsInProgress;

	const TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Job.Url);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("User-Agent"), TEXT("CountriesFlags/1.0"));
	
	Request->OnProcessRequestComplete().BindUObject(this, &UCTCountriesEditorSubsystem::OnImageDownloaded, Job);
	Request->ProcessRequest();
}

void UCTCountriesEditorSubsystem::OnImageDownloaded(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, FImageJob Job)
{
	ON_SCOPE_EXIT
	{
		ProcessDownloads();
	};
	
	--JobsInProgress;

	UCTCountryPrimaryDataAsset* DataAsset = Job.DataAsset.Get();
	if (!IsValid(DataAsset))
	{
		UE_LOG(LogCountriesEditor, Warning, TEXT("Data asset was destroyed before image could be downloaded: %s"), *Job.Url);
		return;
	}
	
	if (!bConnectedSuccessfully || !Response.IsValid() || Response->GetResponseCode() != 200)
	{
		UE_LOG(LogCountriesEditor, Warning, TEXT("Download failed: %s"), *Job.Url);
		return;
	}
		
	UTexture2D* Texture = CreateTextureAssetFromPng(Response->GetContent(), Job.PackageName, Job.AssetName);
	if (!IsValid(Texture))
	{
		UE_LOG(LogCountriesEditor, Warning, TEXT("Failed to decode image: %s"), *Job.Url);
		return;
	}
		
	SaveAssetPackage(Texture);

	if (Job.bIsFlag)
	{
		DataAsset->FlagTexture = Texture;
	}
	else
	{
		DataAsset->CoatOfArmsTexture = Texture;
	}
	
	(void)DataAsset->MarkPackageDirty();
		
	if (int32* Pending = PendingImages.Find(Job.DataAsset))
	{
		if (--*Pending <= 0)
		{
			PendingImages.Remove(Job.DataAsset);
		}
	}
}

UTexture2D* UCTCountriesEditorSubsystem::CreateTextureAssetFromPng(const TArray<uint8>& PngBytes, const FString& PackageName, const FString& AssetName)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));
	const TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(PngBytes.GetData(), PngBytes.Num()))
	{
		return nullptr;
	}

	TArray<uint8> RawBGRA;
	if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawBGRA))
	{
		return nullptr;
	}

	UPackage* Package = CreatePackage(*PackageName);
	if (!IsValid(Package))
	{
		return nullptr;
	}
	
	Package->FullyLoad();

	UTexture2D* Texture = NewObject<UTexture2D>(Package, *AssetName, RF_Public | RF_Standalone);
	Texture->Source.Init(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), 1, 1, TSF_BGRA8, RawBGRA.GetData());
	Texture->SRGB = true;
	Texture->MipGenSettings = TMGS_NoMipmaps;
	Texture->CompressionSettings = TC_Default;
	Texture->UpdateResource();
	Texture->PostEditChange();

	FAssetRegistryModule::AssetCreated(Texture);
	(void)Package->MarkPackageDirty();

	return Texture;
}

void UCTCountriesEditorSubsystem::SaveAssetPackage(UObject* Asset)
{
	if (!IsValid(Asset))
	{
		return;
	}

	UPackage* Package = Asset->GetPackage();
	(void)Package->MarkPackageDirty();

	FSavePackageArgs SaveArguments;
	SaveArguments.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArguments.SaveFlags = SAVE_NoError;

	const FString PackageFileName = FPackageName::LongPackageNameToFilename(Package->GetName(), FPackageName::GetAssetPackageExtension());
	UPackage::SavePackage(Package, Asset, *PackageFileName, SaveArguments);
}

void UCTCountriesEditorSubsystem::SaveAllDataAssets()
{
	for (TObjectPtr<UCTCountryPrimaryDataAsset> DataAsset : CountryDataAssets)
	{
		SaveAssetPackage(DataAsset);
	}
}

void UCTCountriesEditorSubsystem::FinishExport()
{
	SaveAllDataAssets();
	
	IsExporting = false;
	StopNotification();
	
	UE_LOG(LogCountriesEditor, Log, TEXT("Finished exporting countries."));
}

void UCTCountriesEditorSubsystem::StartNotification()
{
	FString BasePath = GetDefault<UCTCountriesDeveloperSettings>()->BasePackagePath;
	
	FNotificationInfo Info(FText::FromString(FString::Printf(TEXT("Exporting countries to %s..."), *BasePath)));
	Info.bFireAndForget = false;
	Info.FadeOutDuration = 0.0f;
	Info.ExpireDuration = 0.0f;

	NotificationItem = FSlateNotificationManager::Get().AddNotification(Info);
}

void UCTCountriesEditorSubsystem::StopNotification()
{
	if (!NotificationItem.IsValid())
	{
		return;
	}
	
	NotificationItem->SetText(FText::FromString(TEXT("Finished exporting countries!")));
	NotificationItem->SetExpireDuration(5.0f);
	NotificationItem.Reset();
}
