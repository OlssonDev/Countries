// Copyright (c) OlssonDev 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CTCountriesDeveloperSettings.generated.h"

UCLASS(config = Editor, defaultconfig)
class UCTCountriesDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	// The path where country data will be exported to. 
	UPROPERTY(EditAnywhere, Config, Category = "Countries Editor")
	FString BasePackagePath = TEXT("/Game/Countries");
	
	// The prefix for flag texture assets. The country name will be appended to this prefix to form the final asset name.
	UPROPERTY(EditAnywhere, Config, Category = "Countries Editor")
	FString FlagTexturePrefix = TEXT("T_Flag_");
	
	// The prefix for coat of arms texture assets. The country name will be appended to this prefix to form the final asset name.
	UPROPERTY(EditAnywhere, Config, Category = "Countries Editor")
	FString CoatOfArmsTexturePrefix = TEXT("T_CoatOfArms_");
	
	// The maximum number of concurrent image downloads when exporting country data.
	UPROPERTY(EditAnywhere, Config, Category = "Countries Editor")
	int32 MaxConcurrentDownloads = 6;
};
