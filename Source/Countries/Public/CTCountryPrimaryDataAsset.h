// Copyright (c) OlssonDev 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CTCountryInfo.h"
#include "Engine/DataAsset.h"
#include "CTCountryPrimaryDataAsset.generated.h"

UCLASS(BlueprintType, NotBlueprintable, Const, DisplayName="Country Data Asset")
class UCTCountryPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	// UObject interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// End of UObject interface
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country", meta = (ShowOnlyInnerProperties))
	FCTCountryInfo CountryInfo;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Flags")
	TSoftObjectPtr<UTexture2D> FlagTexture;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Flags")
	TSoftObjectPtr<UTexture2D> CoatOfArmsTexture;
};

