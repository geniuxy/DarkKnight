// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkInventoryItemManifest.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Object.h"
#include "DkInventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryItem : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }
	bool IsItemStackable() const;
	bool DoesItemTagMatch(const FGameplayTag& ItemTag) const;

	void SetItemManifest(const FInventoryItemManifest& Manifest);
	const FInventoryItemManifest& GetItemManifest() const { return ItemManifest.Get<FInventoryItemManifest>(); }
	FInventoryItemManifest& GetItemManifestMutable() { return ItemManifest.GetMutable<FInventoryItemManifest>(); }

private:
	UPROPERTY(VisibleAnywhere, meta=(BaseStruct="/Script/DarkKnight.InventoryItemManifest"), Replicated)
	FInstancedStruct ItemManifest;

	UPROPERTY(Replicated)
	int32 TotalStackCount = 0;

public:
	LIST_DATA_ACCESSOR(int32, TotalStackCount)
};

template<typename FragmentType>
const FragmentType* GetFragment(const UDkInventoryItem* Item, const FGameplayTag& Tag)
{
	if (!IsValid(Item)) return nullptr;

	const FInventoryItemManifest& Manifest = Item->GetItemManifest();
	return Manifest.GetFragmentOfTypeWithTag<FragmentType>(Tag);
}
