// Copyright (c) OlssonDev 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CTCountriesSubsystem.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CTFetchAllCountriesAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncOnCountriesFetched, const TArray<FCTCountryInfo>&, Countries, bool, bSuccess);
DECLARE_LOG_CATEGORY_EXTERN(LogCTFetchAllCountriesAsyncAction, Log, All);

UCLASS()
class UCTFetchAllCountriesAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;
	// End of UBlueprintAsyncActionBase interface
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Countries")
	static UCTFetchAllCountriesAsyncAction* FetchAllCountries(UObject* WorldContextObject);
	
private:
	UGameInstance* GetGameInstance() const;
	void OnCountriesFetched(const TArray<FCTCountryInfo>& Countries, bool bSuccess);
	
	UPROPERTY(BlueprintAssignable, meta = (AllowPrivate))
	FAsyncOnCountriesFetched OnCompleted;
};
