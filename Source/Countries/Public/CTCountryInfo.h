#pragma once

#include "CoreMinimal.h"
#include "CTCountryInfo.generated.h"

USTRUCT(BlueprintType)
struct FCTNativeName
{
	GENERATED_BODY()
	
	// Official name in the native language.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Name")
	FString Official;
	
	// Common name in the native language.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Name")
	FString Common;
};

USTRUCT(BlueprintType)
struct FCTCountryName
{
	GENERATED_BODY()
	
	// The common name of the country.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Name")
	FString Common;
	
	// The official name of the country.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Name")
	FString Official;
	
	// A map of language codes to native naming structures.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Name")
	TMap<FString, FCTNativeName> NativeName;
};

USTRUCT(BlueprintType)
struct FCTCurrency
{
	GENERATED_BODY()
	
	// The name of the currency.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Currency")
	FString Name;
	
	// The currency symbol (e.g., $, €, ¥).
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Currency")
	FString Symbol;
};

USTRUCT(BlueprintType)
struct FCTIdd
{
	GENERATED_BODY()
	
	// The root of the international dialing code.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Dialling")
	FString Root;
	
	// Valid suffixes for the dialing code.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Dialling")
	TArray<FString> Suffixes;
};

USTRUCT(BlueprintType)
struct FCTCapitalInfo
{
	GENERATED_BODY()

	// Latitude and longitude coordinates of the capital.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Geo")
	TArray<double> Latlng;
};

USTRUCT(BlueprintType)
struct FCTTranslation
{
	GENERATED_BODY()
	
	// Official translated name.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Translation")
	FString Official;
	
	// Common translated name.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Translation")
	FString Common;
};

USTRUCT(BlueprintType)
struct FCTDemonym
{
	GENERATED_BODY()
	
	// Female form of the demonym.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Demonym")
	FString F;
	
	// Male form of the demonym.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Demonym")
	FString M;
};

USTRUCT(BlueprintType)
struct FCTFlags
{
	GENERATED_BODY()
	
	// URL to the PNG flag image.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Flag")
	FString Png;
	
	// URL to the SVG flag image.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Flag")
	FString Svg;
	
	// Alt text describing the flag.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Flag")
	FString Alt;
};

USTRUCT(BlueprintType)
struct FCTCoatOfArms
{
	GENERATED_BODY()
	
	// URL to the PNG coat of arms image.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|CoatOfArms")
	FString Png;
	
	// URL to the SVG coat of arms image.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|CoatOfArms")
	FString Svg;
};

USTRUCT(BlueprintType)
struct FCTMaps
{
	GENERATED_BODY()
	
	// URL to the Google Maps view.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Maps")
	FString GoogleMaps;
	
	// URL to the OpenStreetMap view.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Maps")
	FString OpenStreetMaps;
};

USTRUCT(BlueprintType)
struct FCTCar
{
	GENERATED_BODY()
	
	// List of international vehicle registration codes.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Car")
	TArray<FString> Signs;
	
	// The side of the road on which traffic drives.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Car")
	FString Side;
};

USTRUCT(BlueprintType)
struct FCTPostalCode
{
	GENERATED_BODY()
	
	// The standard format for postal codes.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Postal")
	FString Format;
	
	// Regular expression to validate postal codes.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country|Postal")
	FString Regex;
};

USTRUCT(BlueprintType)
struct FCTCountryInfo
{
	GENERATED_BODY()
	
	// Official, common, and native name variations.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FCTCountryName Name;

	// List of Top Level Domains (e.g., .us, .se).
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TArray<FString> Tld;

	// ISO 3166-1 alpha-2 two-letter country code.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FString Cca2;

	// ISO 3166-1 numeric three-digit country code.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FString Ccn3;

	// ISO 3166-1 alpha-3 three-letter country code.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FString Cca3;

	// International Olympic Committee country code.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FString Cioc;

	// Fédération Internationale de Football Association country code.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FString Fifa;

	// Alternative spellings for the country name.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TArray<FString> AltSpellings;

	// Whether the country is internationally recognized as independent.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	bool Independent = false;
	
	// Political status (e.g., officially assigned, user-assigned).
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FString Status;
 
	// Whether the country is a member of the United Nations.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	bool UNMember = false;

	// Map of currency codes to currency details.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TMap<FString, FCTCurrency> Currencies;

	// International Direct Dialing codes.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FCTIdd Idd;
	
	// List of capital cities.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TArray<FString> Capital;
	
	// Geographic coordinates of the capital.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FCTCapitalInfo CapitalInfo;
	
	// The broader geographic region.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FString Region;
	
	// The specific subregion within the continent.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FString Subregion;
	
	// List of continents this country belongs to.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TArray<FString> Continents;
	
	// Latitude and longitude of the country.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TArray<double> Latlng;
	
	// Whether the country has no direct access to an ocean.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	bool Landlocked = false;
	
	// List of country codes that share a land border.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TArray<FString> Borders;
	
	// Total land area in square kilometers.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	double Area = 0.0;
	
	// List of timezones in the country.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TArray<FString> Timezones;
	
	// Map of language codes to language names.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TMap<FString, FString> Languages;

	// Country name translations in various languages.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TMap<FString, FCTTranslation> Translations;
	
	// Emoji representation of the flag.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FString Flag;

	// URLs for PNG/SVG flag assets.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FCTFlags Flags;
	
	// URLs for coat of arms assets.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FCTCoatOfArms CoatOfArms;
	
	// Gendered demonyms for citizens.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TMap<FString, FCTDemonym> Demonyms;
	
	// Total estimated population.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	int64 Population = 0;
	
	// Gini coefficient data for income inequality.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	TMap<FString, double> Gini;
	
	// External map service links.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FCTMaps Maps;
	
	// Driving side and vehicle identification signs.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FCTCar Car;
	
	// Postal code regex and formatting rules.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FCTPostalCode PostalCode;
	
	// The day of the week the work/school week begins.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Country")
	FString StartOfWeek;
};