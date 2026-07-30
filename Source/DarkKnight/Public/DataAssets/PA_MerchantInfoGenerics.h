// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkStructs.h"
#include "Engine/DataAsset.h"
#include "PA_MerchantInfoGenerics.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UPA_MerchantInfoGenerics : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Product Details")
	TArray<FShopItemEntry> ShopItemEntries;

public:
	FORCEINLINE TArray<FShopItemEntry> GetShopItemEntries() const { return ShopItemEntries; }
};
