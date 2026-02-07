// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkEquipmentComponent.h"

#include "Characters/DkCharacterBase.h"
#include "Components/DkInventoryComponent.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"


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
}

void UDkEquipmentComponent::OnItemUnEquipped(UDkInventoryItem* EquippedItem)
{
}


