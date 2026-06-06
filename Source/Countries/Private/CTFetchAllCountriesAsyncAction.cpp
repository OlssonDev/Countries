// Copyright (c) OlssonDev 2026. All Rights Reserved.


#include "CTFetchAllCountriesAsyncAction.h"

DEFINE_LOG_CATEGORY(LogCTFetchAllCountriesAsyncAction)

void UCTFetchAllCountriesAsyncAction::Activate()
{
	Super::Activate();
	GetGameInstance()->RegisterReferencedObject(this);
}

void UCTFetchAllCountriesAsyncAction::SetReadyToDestroy()
{
	Super::SetReadyToDestroy();
	
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		GameInstance->UnregisterReferencedObject(this);
	}
};

UCTFetchAllCountriesAsyncAction* UCTFetchAllCountriesAsyncAction::FetchAllCountries(UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogCTFetchAllCountriesAsyncAction, Error, TEXT("Invalid WorldContextObject provided to FetchAllCountries."));
		return nullptr;
	}
	
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogCTFetchAllCountriesAsyncAction, Error, TEXT("Could not retrieve GameInstance from WorldContextObject in FetchAllCountries."));
		return nullptr;
	}
	
	UCTFetchAllCountriesAsyncAction* Action = NewObject<UCTFetchAllCountriesAsyncAction>(WorldContextObject);
	Action->Activate();
	
	UCTCountriesSubsystem* CountriesSubsystem = GEngine->GetEngineSubsystem<UCTCountriesSubsystem>();
	CountriesSubsystem->FetchAllCountries(FOnCountriesFetched::CreateUObject(Action, &UCTFetchAllCountriesAsyncAction::OnCountriesFetched));
	
	return Action;
}

UGameInstance* UCTFetchAllCountriesAsyncAction::GetGameInstance() const
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return nullptr;
	}
	
	return World->GetGameInstance();
}

void UCTFetchAllCountriesAsyncAction::OnCountriesFetched(const TArray<FCTCountryInfo>& Countries, bool bSuccess)
{
	OnCompleted.Broadcast(Countries, bSuccess);
	SetReadyToDestroy();
}
