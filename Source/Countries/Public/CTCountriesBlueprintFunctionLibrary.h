// Copyright (c) OlssonDev 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CTCountryInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CTCountriesBlueprintFunctionLibrary.generated.h"

UCLASS(DisplayName="Countries Utility Library")
class UCTCountriesBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Countries")
	void OpenCountryInGoogleMaps(const FCTCountryInfo& Country);
	
	UFUNCTION(BlueprintCallable, Category = "Countries")
	void OpenCountryInOpenStreetMaps(const FCTCountryInfo& Country);
	
	UFUNCTION(BlueprintCallable, Category = "Countries")
	void OpenCountryInMaps(const FCTCountryInfo& Country, bool bPreferGoogleMaps);
	
	UFUNCTION(BlueprintCallable, Category = "Countries")
	void OpenFlagInBrowser(const FCTCountryInfo& Country);
	
	UFUNCTION(BlueprintCallable, Category = "Countries")
	void OpenCoatOfArmsInBrowser(const FCTCountryInfo& Country);
};
