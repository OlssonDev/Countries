#include "CTCountriesSubsystem.h"

#include "CTCountryInfo.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Engine/Texture2D.h"
#include "Modules/ModuleManager.h"

void UCTCountriesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	FOnCountriesFetched OnFetched;
	FetchAllCountries(OnFetched);
}

void UCTCountriesSubsystem::K2_FetchAllCountries(FK2_OnCountriesFetched OnCompleted)
{
	FOnCountriesFetched NativeOnCompleted;
	NativeOnCompleted.BindUFunction(OnCompleted.GetUObject(), OnCompleted.GetFunctionName());
	
	FetchAllCountries(NativeOnCompleted);
}

void UCTCountriesSubsystem::FetchAllCountries(const FOnCountriesFetched& OnCompleted)
{
	using namespace Countries::Internal;
	
	const TSharedPtr<FCountriesFetchState> State = MakeShared<FCountriesFetchState>();
	State->PendingRequests = FieldBatches.Num();

	for (const FString& Fields : FieldBatches)
	{
		SendCountriesInfoRequest(Fields, State, OnCompleted);
	}
}

void UCTCountriesSubsystem::SendCountriesInfoRequest(const FString& Fields, const TSharedPtr<Countries::Internal::FCountriesFetchState>& State, const FOnCountriesFetched& OnCompleted)
{
	const TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(FString::Printf(TEXT("https://restcountries.com/v3.1/all?fields=%s"), *Fields));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("User-Agent"), TEXT("UnrealCountryFlags/1.0"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->OnProcessRequestComplete().BindUObject(this, &UCTCountriesSubsystem::OnCountriesInfoRequestReceived, State, OnCompleted);
	Request->ProcessRequest();
}

void UCTCountriesSubsystem::OnCountriesInfoRequestReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, TSharedPtr<Countries::Internal::FCountriesFetchState> State, FOnCountriesFetched OnCompleted)
{
	if (!bConnectedSuccessfully || !Response.IsValid() || Response->GetResponseCode() != 200)
	{
		State->AnyErrorOccured = true;
	}
	else
	{
		TArray<TSharedPtr<FJsonValue>> JsonArray;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonArray))
		{
			for (const TSharedPtr<FJsonValue>& Value : JsonArray)
			{
				const TSharedPtr<FJsonObject>* Obj = nullptr;
				if (!Value.IsValid() || !Value->TryGetObject(Obj))
				{
					continue;
				}

				FString Cca3;
				if (!(*Obj)->TryGetStringField(TEXT("cca3"), Cca3) || Cca3.IsEmpty())
				{
					continue;
				}

				TSharedPtr<FJsonObject>& Dest = State->MergedRequests.FindOrAdd(Cca3);
				if (!Dest.IsValid())
				{
					Dest = MakeShared<FJsonObject>();
				}
						
				for (const TPair<FString, TSharedPtr<FJsonValue>>& Pair : (*Obj)->Values)
				{
					Dest->SetField(Pair.Key, Pair.Value);
				}
			}
		}
		else
		{
			State->AnyErrorOccured = true;
		}
	}

	if (--State->PendingRequests > 0)
	{
		return;
	}
		
	TArray<FCTCountryInfo> Countries;
	Countries.Reserve(State->MergedRequests.Num());
		
	for (const TPair<FString, TSharedPtr<FJsonObject>>& Pair : State->MergedRequests)
	{
		FCTCountryInfo Info;
		FJsonObjectConverter::JsonObjectToUStruct(Pair.Value.ToSharedRef(), FCTCountryInfo::StaticStruct(), &Info, 0, 0);
		
		Countries.Add(MoveTemp(Info));
	}

	OnCompleted.ExecuteIfBound(Countries, !State->AnyErrorOccured && Countries.Num() > 0);
}
