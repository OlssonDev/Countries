// Copyright (c) OlssonDev 2026. All Rights Reserved.

#include "CTCountryPrimaryDataAsset.h"

FPrimaryAssetId UCTCountryPrimaryDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Country", GetFName());
}
