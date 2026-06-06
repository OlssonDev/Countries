// Copyright (c) OlssonDev 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Subsystems/EngineSubsystem.h"
#include "CTCountriesSubsystem.generated.h"

struct FCTCountryInfo;

DECLARE_DELEGATE_TwoParams(FOnCountriesFetched, const TArray<FCTCountryInfo>&, bool);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FK2_OnCountriesFetched, const TArray<FCTCountryInfo>&, Countries, bool, bSuccess);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountriesExportedAssets, bool, bSuccess);

namespace Countries::Internal
{
	const TArray<FString> FieldBatches = 
	{
		TEXT("cca3,name,tld,cca2,ccn3,cioc,fifa,independent,status,unMember"),
		TEXT("cca3,currencies,idd,capital,altSpellings,region,subregion,languages,latlng,landlocked"),
		TEXT("cca3,borders,area,demonyms,flag,flags,maps,population,gini,car"),
		TEXT("cca3,timezones,continents,coatOfArms,startOfWeek,capitalInfo,postalCode,translations")
	};
	
	struct FCountriesFetchState
	{
		int32 PendingRequests = 0;
		bool AnyErrorOccured = false;
		TMap<FString, TSharedPtr<FJsonObject>> MergedRequests;
	};
}

UCLASS(DisplayName = "Countries Subsystem")
class UCTCountriesSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// End of USubsystem interface
	
	UFUNCTION(BlueprintCallable, Category = "Country Flags")
	COUNTRIES_API void K2_FetchAllCountries(FK2_OnCountriesFetched OnCompleted);
	COUNTRIES_API void FetchAllCountries(const FOnCountriesFetched& OnCompleted);
	
private:
	void SendCountriesInfoRequest(const FString& Fields, const TSharedPtr<Countries::Internal::FCountriesFetchState>& State, const FOnCountriesFetched& OnCompleted);
	void OnCountriesInfoRequestReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, TSharedPtr<Countries::Internal::FCountriesFetchState> State, FOnCountriesFetched OnCompleted);
};
