// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DkInventoryItem.h"

#include "Inventory/DkInventoryItemFragment.h"
#include "Net/UnrealNetwork.h"

void UDkInventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

bool UDkInventoryItem::IsItemStackable() const
{
	const FInventoryItemStackableFragment* StackableFragment =
		GetItemManifest().GetFragmentOfType<FInventoryItemStackableFragment>();
	return StackableFragment != nullptr;
}

void UDkInventoryItem::SetItemManifest(const FInventoryItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FInventoryItemManifest>(Manifest);
}
