// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkEquipmentComponent.h"

#include "Characters/DkCharacterBase.h"
#include "Components/DkInventoryComponent.h"
#include "Equipment/DkEquippedActorBase.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"


UDkEquipmentComponent::UDkEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UDkEquipmentComponent::InitializeOwner(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		OwningController = PlayerController;
		InitInventoryComponent();
	}
}

void UDkEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerCharacter();
}

void UDkEquipmentComponent::InitPlayerCharacter()
{
	// 能BeginPlay，说明OwningCharacter（GetOwner()）肯定是有效的，因为EquipmentComponent是Character的一部分
	// 对于PreviewActor则是无效的，所以需要另外定义Controller、SkeletalMesh等
	OwningCharacter = Cast<ADkCharacterBase>(GetOwner());
	if (OwningCharacter.IsValid())
	{
		OwningSkeletalMesh = OwningCharacter->GetMesh();
		OwningController = Cast<APlayerController>(OwningCharacter->GetController());
		if (OwningController.IsValid())
		{
			InitInventoryComponent();
		}
		else
		{
			OwningCharacter->ReceiveControllerChangedDelegate.AddDynamic(this, &ThisClass::OnControllerChanged);
		}
	}
}

void UDkEquipmentComponent::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	if (APlayerController* NewPlayerController = Cast<APlayerController>(NewController); IsValid(NewPlayerController))
	{
		OwningController = NewPlayerController;
		InitInventoryComponent();
	}
}

void UDkEquipmentComponent::InitInventoryComponent()
{
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

void UDkEquipmentComponent::OnItemEquipped(UDkInventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem)) return;
	if (!OwningCharacter->HasAuthority()) return;

	FInventoryItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FInventoryItemEquipmentFragment* EquipmentFragment =
		ItemManifest.GetFragmentOfTypeMutable<FInventoryItemEquipmentFragment>();
	if (!EquipmentFragment) return;
	
	if (!bIsPreview) // 预览时不需要调整属性等操作
	{
		EquipmentFragment->OnEquip(OwningController.Get());
	}

	if (OwningSkeletalMesh.IsValid())
	{
		ADkEquippedActorBase* SpawnedEquippedActor = SpawnEquippedActor(EquipmentFragment, OwningSkeletalMesh.Get());
		EquippedActors.Add(SpawnedEquippedActor);
	}
}

void UDkEquipmentComponent::OnItemUnEquipped(UDkInventoryItem* UnEquippedItem)
{
	if (!IsValid(UnEquippedItem)) return;
	if (!OwningCharacter->HasAuthority()) return;

	FInventoryItemManifest& ItemManifest = UnEquippedItem->GetItemManifestMutable();
	FInventoryItemEquipmentFragment* EquipmentFragment =
		ItemManifest.GetFragmentOfTypeMutable<FInventoryItemEquipmentFragment>();
	if (!EquipmentFragment) return;
	
	if (!bIsPreview) // 预览时不需要调整属性等操作
	{
		EquipmentFragment->OnUnEquip(OwningController.Get());
	}

	RemoveEquippedActor(EquipmentFragment->GetEquipmentTag());
}

ADkEquippedActorBase* UDkEquipmentComponent::SpawnEquippedActor(
	FInventoryItemEquipmentFragment* EquipmentFragment, USkeletalMeshComponent* AttachMesh)
{
	ADkEquippedActorBase* SpawnedEquippedActor = EquipmentFragment->SpawnAttachActor(AttachMesh);
	SpawnedEquippedActor->SetEquipmentTag(EquipmentFragment->GetEquipmentTag());
	SpawnedEquippedActor->SetOwner(GetOwner());
	EquipmentFragment->SetEquippedActor(SpawnedEquippedActor);
	return SpawnedEquippedActor;
}

ADkEquippedActorBase* UDkEquipmentComponent::FindEquippedActor(const FGameplayTag& EquippedActorTag)
{
	auto FoundActor = EquippedActors.FindByPredicate(
		[EquippedActorTag](const ADkEquippedActorBase* EquippedActor)
		{
			return EquippedActor->GetEquipmentTag().MatchesTagExact(EquippedActorTag);
		}
	);
	return FoundActor ? *FoundActor : nullptr;
}

void UDkEquipmentComponent::RemoveEquippedActor(const FGameplayTag& EquippedActorTag)
{
	if (ADkEquippedActorBase* EquippedActor = FindEquippedActor(EquippedActorTag); IsValid(EquippedActor))
	{
		EquippedActors.Remove(EquippedActor);
		EquippedActor->Destroy();
	}
}
