// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DkInventoryItem.h"

#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Net/UnrealNetwork.h"

void UDkInventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
	DOREPLIFETIME(ThisClass, TotalStackCount);
}

bool UDkInventoryItem::IsItemStackable() const
{
	const FItemFragment_Stackable* StackableFragment =
		GetItemManifest().GetFragmentOfType<FItemFragment_Stackable>();
	return StackableFragment != nullptr;
}

int UDkInventoryItem::GetItemId() const
{
	return GetItemManifest().GetItemID();
}

FText UDkInventoryItem::GetItemName() const
{
	return UDkInventoryFunctionLibrary::GetItemName(GetItemManifest().GetItemID());
}

FGameplayTag UDkInventoryItem::GetItemTag() const
{
	return UDkInventoryFunctionLibrary::GetItemTag(GetItemManifest().GetItemID());
}

EInventoryItemCategory UDkInventoryItem::GetItemCategory() const
{
	return UDkInventoryFunctionLibrary::GetItemCategory(GetItemManifest().GetItemID());
}

int UDkInventoryItem::GetItemStackCount() const
{
	return GetItemManifest().GetStackCount();
}

bool UDkInventoryItem::DoesItemTagMatch(const FGameplayTag& ItemTag) const
{
	return GetItemManifest().GetItemTag().MatchesTagExact(ItemTag);
}

void UDkInventoryItem::SetItemManifest(const FInventoryItemManifest& Manifest)
{
	// 这里复制了一份Manifest给ItemManifest
	ItemManifest = FInstancedStruct::Make<FInventoryItemManifest>(Manifest);
}

void UDkInventoryItem::SetItemManifest(const FDkItemInfo& ItemInfo, int32 InItemStack)
{
	FInventoryItemManifest Manifest;
	Manifest.SetItemID(ItemInfo.ItemID);
	Manifest.SetItemCategory(ItemInfo.ItemCategory);
	Manifest.SetItemTag(ItemInfo.ItemTag);
	Manifest.InitializeFragments(ItemInfo, InItemStack);
	
	ItemManifest = FInstancedStruct::Make<FInventoryItemManifest>(Manifest);
}
