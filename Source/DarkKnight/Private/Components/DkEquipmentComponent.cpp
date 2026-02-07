// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkEquipmentComponent.h"

#include "Characters/DkCharacterBase.h"
#include "Components/DkInventoryComponent.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"


UDkEquipmentComponent::UDkEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UDkEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ADkCharacterBase>(GetOwner());
	if (OwningCharacter.IsValid())
	{
		OwningController = Cast<APlayerController>(OwningCharacter->GetController());
		OwningSkeletalMesh = OwningCharacter->GetMesh();

		// 初始化InventoryComponent
		InventoryComponent = UDkInventoryFunctionLibrary::GetInventoryComponent(OwningController.Get());
		if (!InventoryComponent.IsValid()) return;
		if (!InventoryComponent->OnItemEquipped.IsAlreadyBound(this, &ThisClass::OnItemEquipped))
		{
			InventoryComponent->OnItemEquipped.AddDynamic(this, &ThisClass::OnItemEquipped);
		}
		if (!InventoryComponent->OnItemUnEquipped.IsAlreadyBound(this, &ThisClass::OnItemUnEquipped))
		{
			InventoryComponent->OnItemUnEquipped.AddDynamic(this, &ThisClass::OnItemUnEquipped);
		}
	}
}

void UDkEquipmentComponent::OnItemEquipped(UDkInventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem)) return;
	if (!OwningCharacter->HasAuthority()) return;

	FInventoryItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FInventoryItemEquipmentFragment* EquipmentFragment =
		ItemManifest.GetFragmentOfTypeMutable<FInventoryItemEquipmentFragment>();
	if (EquipmentFragment)
	{
		EquipmentFragment->OnEquip(OwningController.Get());
	}
}

void UDkEquipmentComponent::OnItemUnEquipped(UDkInventoryItem* UnEquippedItem)
{
	if (!IsValid(UnEquippedItem)) return;
	if (!OwningCharacter->HasAuthority()) return;

	FInventoryItemManifest& ItemManifest = UnEquippedItem->GetItemManifestMutable();
	FInventoryItemEquipmentFragment* EquipmentFragment =
		ItemManifest.GetFragmentOfTypeMutable<FInventoryItemEquipmentFragment>();
	if (EquipmentFragment)
	{
		EquipmentFragment->OnUnEquip(OwningController.Get());
	}
}


