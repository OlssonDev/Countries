// Copyright (c) OlssonDev 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CTCountryInfo.h"
#include "EditorSubsystem.h"
#include "Interfaces/IHttpRequest.h"
#include "CTCountriesEditorSubsystem.generated.h"

class UCTCountryPrimaryDataAsset;

namespace Countries::Http
{
	struct FImageJob
	{
		FString Url;
		FString PackageName;
		FString AssetName;
		TWeakObjectPtr<UCTCountryPrimaryDataAsset> DataAsset;
		bool bIsFlag = true;
	};
}

UCLASS()
class UCTCountriesEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	void ExportCountriesAsDataAssets();
private:
	using FImageJob = Countries::Http::FImageJob;
	void OnAllCountriesFetched(const TArray<FCTCountryInfo>& Countries, bool bSuccess);
    
	void ProcessDownloads();
	void StartDownloadJob(const FImageJob& Job);
	void OnImageDownloaded(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, FImageJob Job);
    
	UTexture2D* CreateTextureAssetFromPng(const TArray<uint8>& PngBytes, const FString& PackageName, const FString& AssetName);
	static void SaveAssetPackage(UObject* Asset);
	
	void SaveAllDataAssets();
	void FinishExport();
	
	void StartNotification();
	void StopNotification();
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UCTCountryPrimaryDataAsset>> CountryDataAssets;

	TArray<FImageJob> JobQueue;
	int32 CurrentJobIndex = 0;
	int32 JobsInProgress = 0;
	
	bool IsExporting = false;
	
	TSharedPtr<SNotificationItem> NotificationItem;
    
	TMap<TWeakObjectPtr<UCTCountryPrimaryDataAsset>, int32> PendingImages;
};
