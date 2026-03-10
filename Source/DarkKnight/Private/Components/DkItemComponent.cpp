// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkItemComponent.h"

#include "Net/UnrealNetwork.h"

UDkItemComponent::UDkItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PickUpMessage = TEXT("F - 捡起");

	SetIsReplicatedByDefault(true);
}

void UDkItemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

void UDkItemComponent::OnPickedUp()
{
	BP_OnPickedUp();

	GetOwner()->Destroy();
}

void UDkItemComponent::InitializeItemComponent(const FDkItemInfo* ItemInfo, int32 InItemStack)
{
	ItemManifest.SetItemID(ItemInfo->ItemID);
	ItemManifest.SetItemCategory(ItemInfo->ItemCategory);
	ItemManifest.SetItemTag(ItemInfo->ItemTag);
	ItemManifest.InitializeFragments(GetOwner(), ItemInfo, InItemStack);
}

void UDkItemComponent::InitItemManifest(FInventoryItemManifest InItemManifest)
{
	ItemManifest = InItemManifest;
}
