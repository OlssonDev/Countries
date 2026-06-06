// Copyright (c) OlssonDev 2026. All Rights Reserved.

#include "CTCountriesBlueprintFunctionLibrary.h"
#include "Countries.h"

void UCTCountriesBlueprintFunctionLibrary::OpenCountryInGoogleMaps(const FCTCountryInfo& Country)
{
	FString Url = Country.Maps.GoogleMaps;
	
	if (Url.IsEmpty())
	{
		UE_LOG(LogCountries, Warning, TEXT("No Google Maps URL available for country %s"), *Country.Name.Common);
		return;
	}
	
	FPlatformProcess::LaunchURL(*Url, nullptr, nullptr);
}

void UCTCountriesBlueprintFunctionLibrary::OpenCountryInOpenStreetMaps(const FCTCountryInfo& Country)
{
	FString Url = Country.Maps.OpenStreetMaps;
	
	if (Url.IsEmpty())
	{
		UE_LOG(LogCountries, Warning, TEXT("No OpenStreetMap URL available for country %s"), *Country.Name.Common);
		return;
	}
	
	FPlatformProcess::LaunchURL(*Url, nullptr, nullptr);
}

void UCTCountriesBlueprintFunctionLibrary::OpenCountryInMaps(const FCTCountryInfo& Country, bool bPreferGoogleMaps)
{
	FString Url = bPreferGoogleMaps ? Country.Maps.GoogleMaps : Country.Maps.OpenStreetMaps;
	
	if (Url.IsEmpty())
	{
		Url = bPreferGoogleMaps ? Country.Maps.OpenStreetMaps : Country.Maps.GoogleMaps;
		
		if (Url.IsEmpty())
		{
			UE_LOG(LogCountries, Warning, TEXT("No map URL available for country %s"), *Country.Name.Common);
			return;
		}
	}
	
	FPlatformProcess::LaunchURL(*Url, nullptr, nullptr);
}

void UCTCountriesBlueprintFunctionLibrary::OpenFlagInBrowser(const FCTCountryInfo& Country)
{
	FString Url = Country.Flags.Png;
	
	if (Url.IsEmpty())
	{
		UE_LOG(LogCountries, Warning, TEXT("No flag image URL available for country %s"), *Country.Name.Common);
		return;
	}
	
	FPlatformProcess::LaunchURL(*Url, nullptr, nullptr);
}

void UCTCountriesBlueprintFunctionLibrary::OpenCoatOfArmsInBrowser(const FCTCountryInfo& Country)
{
	FString Url = Country.CoatOfArms.Png;
	
	if (Url.IsEmpty())
	{
		UE_LOG(LogCountries, Warning, TEXT("No coat of arms image URL available for country %s"), *Country.Name.Common);
		return;
	}
	
	FPlatformProcess::LaunchURL(*Url, nullptr, nullptr);
}
