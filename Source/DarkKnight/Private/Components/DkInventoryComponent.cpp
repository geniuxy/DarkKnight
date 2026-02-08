// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkInventoryComponent.h"

#include "DkGameplayTags.h"
#include "Characters/DkCharacterHero.h"
#include "Components/DkItemComponent.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Net/UnrealNetwork.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "Widgets/GameMenu/DkWidgetGameMenuScreen.h"
#include "Widgets/Inventory/DkWidgetInventoryMenu.h"

UDkInventoryComponent::UDkInventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

void UDkInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UDkInventoryComponent::OnLoadingScreenDeactivated_Implementation()
{
	ConstructInventoryMenu();
}

void UDkInventoryComponent::ConstructInventoryMenu()
{
	if (!bHasInitInventoryMenu)
	{
		OwningCharacter = CastChecked<ADkCharacterHero>(GetOwner());
	}

	if (OwningCharacter.IsValid() && !OwningCharacter->IsLocallyControlled())
	{
		return;
	}

	UDkUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_GameMenu,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_GameMenu),
		[this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly);
			}
			if (InPushState == EAsyncPushWidgetState::AfterPush)
			{
				if (!bHasInitInventoryMenu)
				{
					CachedInventoryMenu = CastChecked<UDkWidgetGameMenuScreen>(PushedWidget)->GetInventoryMenu();
					PushedWidget->DeactivateWidget();
					bHasInitInventoryMenu = true;
				}
			}
		}
	);
}

void UDkInventoryComponent::TryAddItem(UDkItemComponent* ItemComponent)
{
	if (!CachedInventoryMenu.IsValid()) return;
	FDkInventorySlotAvailabilityResult AddItemResult = CachedInventoryMenu->HasRoomForItem(ItemComponent);

	UDkInventoryItem* FoundItem = InventoryList.FindFirstItemByTag(ItemComponent->GetItemManifest().GetItemTag());
	AddItemResult.Item = FoundItem;

	if (AddItemResult.TotalRoomToFill == 0)
	{
		OnNoRoomInInventory.Broadcast();
		return;
	}

	// 将Item添加到Inventory中
	if (AddItemResult.Item.IsValid() && AddItemResult.bStackable)
	{
		// 为背包中已存在的物品添加堆叠数量。我们只想更新堆叠数量，
		// 而不是创建这种类型的新物品。
		OnStackChange.Broadcast(AddItemResult); // OnStackChange在Server和Client都会执行，光在Server执行，无法同步到Client
		Server_AddStacksToItem(ItemComponent, AddItemResult.TotalRoomToFill, AddItemResult.Remainder);
	}
	else
	{
		// 此物品类型在物品栏中不存在。请创建一个新物品并更新所有相关栏位。
		Server_AddNewItem(
			ItemComponent, AddItemResult.bStackable ? AddItemResult.TotalRoomToFill : 0, AddItemResult.Remainder
		);
	}
}

void UDkInventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

void UDkInventoryComponent::Server_AddNewItem_Implementation(
	UDkItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	// 服务器FastArray添加Item后，回调PostReplicatedAdd来达到OnItemAdded.Broadcast(NewItem)的目的，以更新Client
	UDkInventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);
	NewItem->SetTotalStackCount(StackCount);

	if (OwningCharacter.IsValid())
	{
		if (OwningCharacter->GetController()->GetNetMode() == NM_ListenServer ||
			OwningCharacter->GetController()->GetNetMode() == NM_Standalone)
		{
			OnItemAdded.Broadcast(NewItem);
		}
	}

	// 如果Remainder零，则 执行PickedUp操作（销毁 Owner 道具Actor等）
	if (Remainder == 0)
	{
		ItemComponent->OnPickedUp();
	}
	// 否则，更新想要捡起的Item的StackCount
	else if (FInventoryItemStackableFragment* StackableFragment =
		ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FInventoryItemStackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void UDkInventoryComponent::Server_AddStacksToItem_Implementation(
	UDkItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	const FGameplayTag& ItemTag =
		IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemTag() : FGameplayTag::EmptyTag;
	UDkInventoryItem* Item = InventoryList.FindFirstItemByTag(ItemTag);
	if (!IsValid(Item)) return;

	Item->SetTotalStackCount(Item->GetTotalStackCount() + StackCount);

	// 如果Remainder零，则 执行PickedUp操作（销毁 Owner 道具Actor等）
	if (Remainder == 0)
	{
		ItemComponent->OnPickedUp();
	}
	// 否则，更新想要捡起的Item的StackCount
	else if (FInventoryItemStackableFragment* StackableFragment =
		ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FInventoryItemStackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void UDkInventoryComponent::ServerDropItem_Implementation(UDkInventoryItem* Item, int32 DroppedCount)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - DroppedCount;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	SpawnDroppedItem(Item, DroppedCount);
}

void UDkInventoryComponent::SpawnDroppedItem(UDkInventoryItem* Item, int32 DroppedCount)
{
	const AActor* OwningActor = GetOwner();
	FVector RotatedForward = OwningActor->GetActorForwardVector();
	RotatedForward =
		RotatedForward.RotateAngleAxis(FMath::FRandRange(DropSpawnAngleMin, DropSpawnAngleMax), FVector::UpVector);
	FVector SpawnLocation = OwningActor->GetActorLocation() +
		RotatedForward * FMath::FRandRange(DropSpawnDistanceMin, DropSpawnDistanceMax);
	SpawnLocation.Z -= RelativeSpawnElevation;
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	// 通过Item Manifest去生成PickUp的ItemActor
	FInventoryItemManifest& ItemManifest = Item->GetItemManifestMutable();
	if (FInventoryItemStackableFragment* StackableFragment =
		ItemManifest.GetFragmentOfTypeMutable<FInventoryItemStackableFragment>())
	{
		StackableFragment->SetStackCount(DroppedCount);
	}
	ItemManifest.SpawnPickUpActor(this, SpawnLocation, SpawnRotation);
}

void UDkInventoryComponent::ServerConsumeItem_Implementation(UDkInventoryItem* Item)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - 1;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	if (FInventoryItemConsumableFragment* ConsumableFragment =
		Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FInventoryItemConsumableFragment>())
	{
		ACharacter* OwnerCharacter = CastChecked<ACharacter>(GetOwner());
		APlayerController* PlayerController = CastChecked<APlayerController>(OwnerCharacter->GetController());
		ConsumableFragment->OnConsume(PlayerController);
	}
}

void UDkInventoryComponent::ServerUpdateEquippedItem_Implementation(
	UDkInventoryItem* EquippedItem, UDkInventoryItem* UnEquippedItem)
{
	MulticastUpdateEquippedItem(EquippedItem, UnEquippedItem);
}

void UDkInventoryComponent::MulticastUpdateEquippedItem_Implementation(
	UDkInventoryItem* EquippedItem, UDkInventoryItem* UnEquippedItem)
{
	if (IsValid(EquippedItem))
	{
		OnItemEquipped.Broadcast(EquippedItem);
	}

	if (IsValid(UnEquippedItem))
	{
		OnItemUnEquipped.Broadcast(UnEquippedItem);
	}
}

void UDkInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}
