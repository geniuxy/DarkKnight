// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkEquipmentComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "DarkKnightDebugHelper.h"
#include "Characters/DkCharacterBase.h"
#include "Components/InventoryComps/DkPlayerInventoryComp.h"
#include "Equipment/DkEquippedActorBase.h"
#include "Equipment/DkEquippedActorStatic.h"
#include "Equipment/DkEquippedActorStaticWithAdditional.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "GAS/DkAbilitySystemComponent.h"
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

		InitOwnerASC();
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
	if (!OwningController.IsValid()) return;
	if (!bIsPreview && !OwningController->HasAuthority()) return;
	// 预览情况下，本地Client处理PreviewActor的装备/卸装备
	if (bIsPreview && !OwningController->IsLocalController()) return;

	// 初始化当前玩家控制角色的InventoryComponent
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

void UDkEquipmentComponent::InitOwnerASC()
{
	if (OwningCharacter.Get() && OwningCharacter->GetAbilitySystemComponent())
	{
		OwnerASC = Cast<UDkAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())
		);
	}
}

void UDkEquipmentComponent::OnItemEquipped(UDkInventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem)) return;

	FInventoryItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FInventoryItemFragment_Equipment* EquipmentFragment =
		ItemManifest.GetFragmentOfTypeMutable<FInventoryItemFragment_Equipment>();
	if (EquipmentFragment)
	{
		// Tips: 这里PreviewActor和主角是共用一个UDkInventoryItem的，需要修改对另一者没有影响

		if (!bIsPreview && OwnerASC.IsValid()) // 预览时不需要调整属性等操作
		{
			EquipmentFragment->OnEquip(OwnerASC.Get());
		}

		if (OwningSkeletalMesh.IsValid())
		{
			ADkEquippedActorBase* SpawnedEquippedActor =
				SpawnEquippedActor(EquipmentFragment, OwningSkeletalMesh.Get());
			if (SpawnedEquippedActor)
			{
				if (bIsPreview) // 如果是多个Client且为Preview，自己管自己的PreviewActor，其对应的SpawnedEquippedActor不为复制
				{
					SpawnedEquippedActor->SetReplicates(false);
				}
				EquippedActors.Add(SpawnedEquippedActor);
			}
		}
	}

	if (EquippedItem->GetItemManifest().GetItemCategory() == EInventoryItemCategory::Consumable)
	{
		if (bIsPreview) return;

		Debug::Print(TEXT("我装备了消耗品"));
	}
}

void UDkEquipmentComponent::OnItemUnEquipped(UDkInventoryItem* UnEquippedItem)
{
	if (!IsValid(UnEquippedItem)) return;

	FInventoryItemManifest& ItemManifest = UnEquippedItem->GetItemManifestMutable();
	FInventoryItemFragment_Equipment* EquipmentFragment =
		ItemManifest.GetFragmentOfTypeMutable<FInventoryItemFragment_Equipment>();
	if (EquipmentFragment)
	{
		if (!bIsPreview && OwnerASC.IsValid()) // 预览时不需要调整属性等操作
		{
			EquipmentFragment->OnUnEquip(OwnerASC.Get());
		}

		RemoveEquippedActor(EquipmentFragment);
	}

	if (UnEquippedItem->GetItemManifest().GetItemCategory() == EInventoryItemCategory::Consumable)
	{
		if (bIsPreview) return;

		Debug::Print(TEXT("我取下了消耗品"));
	}
}

ADkEquippedActorBase* UDkEquipmentComponent::SpawnEquippedActor(
	FInventoryItemFragment_Equipment* EquipmentFragment, USkeletalMeshComponent* AttachMesh)
{
	ADkEquippedActorBase* SpawnedEquippedActor = EquipmentFragment->SpawnAttachActor(AttachMesh);
	if (IsValid(SpawnedEquippedActor))
	{
		SpawnedEquippedActor->SetOwner(GetOwner());
		EquipmentFragment->SetEquippedActorTag(SpawnedEquippedActor->GetEquipmentTag());
		if (!bIsPreview &&
			SpawnedEquippedActor->GetEquipmentTag().MatchesTag(DkGameplayTags::Dk_Item_Equipment_Weapons_Primary))
		{
			OwnerASC->AuthApplyGameplayEffect(EquippedEffect);
		}
	}

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

ADkEquippedActorBase* UDkEquipmentComponent::FindTargetTypeEquippedActor(const FGameplayTag& TargetTypeTag)
{
	auto FoundActor = EquippedActors.FindByPredicate(
		[TargetTypeTag](const ADkEquippedActorBase* EquippedActor)
		{
			return EquippedActor->GetEquipmentTag().MatchesTag(TargetTypeTag);
		}
	);
	return FoundActor ? *FoundActor : nullptr;
}

void UDkEquipmentComponent::RemoveEquippedActor(FInventoryItemFragment_Equipment* EquipmentFragment)
{
	ADkEquippedActorBase* EquippedActor = FindEquippedActor(EquipmentFragment->GetEquippedActorTag());
	if (IsValid(EquippedActor))
	{
		if (!bIsPreview &&
			EquippedActor->GetEquipmentTag().MatchesTag(DkGameplayTags::Dk_Item_Equipment_Weapons_Primary))
		{
			OwnerASC->RemoveActiveEffectsWithGrantedTags(
				FGameplayTagContainer(DkGameplayTags::Dk_Stats_Equipped_PrimaryWeapon)
			);
		}
		EquippedActors.Remove(EquippedActor);
		EquippedActor->Destroy();
	}
}

void UDkEquipmentComponent::SetIsPreview(bool bInIsPreview)
{
	bIsPreview = bInIsPreview;
	if (bIsPreview)
	{
		SetIsReplicated(false);
	}
}

void UDkEquipmentComponent::InitializeOwner(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		OwningController = PlayerController;
		InitInventoryComponent();
	}
}

void UDkEquipmentComponent::DrawWeapon(FGameplayTag InWeaponTypeTag, FName InSocketName)
{
	ADkEquippedActorBase* TargetTypeEquippedActor = FindTargetTypeEquippedActor(InWeaponTypeTag);
	if (IsValid(TargetTypeEquippedActor))
	{
		ADkEquippedActorStatic* TargetStaticActor = Cast<ADkEquippedActorStatic>(TargetTypeEquippedActor);
		if (TargetStaticActor)
		{
			if (ADkEquippedActorStaticWithAdditional* AdditionalActor =
				Cast<ADkEquippedActorStaticWithAdditional>(TargetTypeEquippedActor))
			{
				AdditionalActor->GetAdditionalMesh()->AttachToComponent(
					OwningSkeletalMesh.Get(),
					FAttachmentTransformRules::SnapToTargetNotIncludingScale,
					InSocketName
				);
			}
			else
			{
				TargetStaticActor->AttachToComponent(
					OwningSkeletalMesh.Get(),
					FAttachmentTransformRules::SnapToTargetNotIncludingScale,
					InSocketName
				);
			}
		}
	}
}

void UDkEquipmentComponent::SheatheWeapon(FGameplayTag InWeaponTypeTag, FName InSocketName)
{
	ADkEquippedActorBase* TargetTypeEquippedActor = FindTargetTypeEquippedActor(InWeaponTypeTag);
	if (IsValid(TargetTypeEquippedActor))
	{
		ADkEquippedActorStatic* TargetStaticActor = Cast<ADkEquippedActorStatic>(TargetTypeEquippedActor);
		if (TargetStaticActor)
		{
			if (ADkEquippedActorStaticWithAdditional* AdditionalActor =
				Cast<ADkEquippedActorStaticWithAdditional>(TargetTypeEquippedActor))
			{
				AdditionalActor->GetAdditionalMesh()->AttachToComponent(
					AdditionalActor->GetEquipmentItemStaticMesh(),
					FAttachmentTransformRules::SnapToTargetNotIncludingScale
				);
			}
			else
			{
				TargetStaticActor->AttachToComponent(
					OwningSkeletalMesh.Get(),
					FAttachmentTransformRules::SnapToTargetNotIncludingScale,
					InSocketName
				);
			}
		}
	}
}
