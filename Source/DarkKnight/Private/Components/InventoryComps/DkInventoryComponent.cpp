// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComps/DkInventoryComponent.h"

#include "DarkKnightDebugHelper.h"
#include "Characters/DkCharacterHero.h"
#include "Components/DkItemComponent.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "AbilitySystemGlobals.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Inventory/DkInventorySlotAvailabilty.h"
#include "Net/UnrealNetwork.h"

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
	DOREPLIFETIME(ThisClass, InventoryCategoryItemsArray);
}

void UDkInventoryComponent::InitializeInventoryComponent()
{
	OwningCharacter = CastChecked<ADkCharacterHero>(GetOwner());
	if (OwningCharacter.IsValid())
	{
		OwnerASC = Cast<UDkAbilitySystemComponent>(
			UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningCharacter.Get())
		);
	}
}

void UDkInventoryComponent::TryAddItem(UDkItemComponent* ItemComponent)
{
	FDkInventorySlotAvailabilityResult AddItemResult = HasRoomForItem(ItemComponent);

	UDkInventoryItem* FoundItem = InventoryList.FindFirstItemByTag(ItemComponent->GetItemManifest().GetItemTag());
	AddItemResult.Item = FoundItem;

	if (AddItemResult.TotalRoomToFill == 0)
	{
		OnNoRoomInInventory.Broadcast(FText::FromString(TEXT("背包中没有足够的空间了")));
		return;
	}

	// 将Item添加到Inventory中
	if (AddItemResult.Item.IsValid() && AddItemResult.bStackable)
	{
		// 为背包中已存在的物品添加堆叠数量。我们只想更新堆叠数量，
		// 而不是创建这种类型的新物品。
		OnAddItemNotice.Broadcast(ItemComponent->GetItemName(), AddItemResult.TotalRoomToFill);
		// OnStackChange.Broadcast(AddItemResult); // OnStackChange在Server和Client都会执行，光在Server执行，无法同步到Client
		Server_AddStacksToItem(ItemComponent, AddItemResult);
	}
	else
	{
		int StackCount = AddItemResult.bStackable ? AddItemResult.TotalRoomToFill : 0;
		OnAddItemNotice.Broadcast(
			ItemComponent->GetItemName(),
			AddItemResult.bStackable ? StackCount : 1
		);
		// 此物品类型在物品栏中不存在。请创建一个新物品并更新所有相关栏位。
		Server_AddNewItem(ItemComponent, StackCount, AddItemResult);
	}
}

void UDkInventoryComponent::TryAddItem(UDkInventoryItem* Item)
{
	FDkInventorySlotAvailabilityResult AddItemResult = HasRoomForItem(Item);

	UDkInventoryItem* FoundItem = InventoryList.FindFirstItemByTag(Item->GetItemManifest().GetItemTag());
	AddItemResult.Item = FoundItem;

	if (AddItemResult.TotalRoomToFill == 0)
	{
		OnNoRoomInInventory.Broadcast(FText::FromString(TEXT("背包中没有足够的空间了")));
		return;
	}

	// 将Item添加到Inventory中
	if (AddItemResult.Item.IsValid() && AddItemResult.bStackable)
	{
		OnAddItemNotice.Broadcast(Item->GetItemName(), AddItemResult.TotalRoomToFill);
		// 为背包中已存在的物品添加堆叠数量。我们只想更新堆叠数量，
		// 而不是创建这种类型的新物品。
		// OnStackChange.Broadcast(AddItemResult); // OnStackChange在Server和Client都会执行，光在Server执行，无法同步到Client
		Server_AddStacksToItemWithItem(Item, AddItemResult);
	}
	else
	{
		int StackCount = AddItemResult.bStackable ? AddItemResult.TotalRoomToFill : 0;
		OnAddItemNotice.Broadcast(
			Item->GetItemName(),
			AddItemResult.bStackable ? StackCount : 1
		);
		// 此物品类型在物品栏中不存在。请创建一个新物品并更新所有相关栏位。
		Server_AddNewItemWithItem(Item, StackCount, AddItemResult);
	}
}

void UDkInventoryComponent::Server_AddNewItem_Implementation(
	UDkItemComponent* ItemComponent, int32 StackCount, FDkInventorySlotAvailabilityResult Result)
{
	// 服务器FastArray添加Item后，回调PostReplicatedAdd来达到OnItemAdded.Broadcast(NewItem)的目的，以更新Client
	UDkInventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);
	NewItem->SetTotalStackCount(StackCount);
	Result.Item = NewItem;

	if (OwningCharacter.IsValid())
	{
		if (OwningCharacter->GetController()->GetNetMode() == NM_ListenServer ||
			OwningCharacter->GetController()->GetNetMode() == NM_Standalone)
		{
			UpdateInventoryCategoryItemsArray(Result);
		}
	}

	// 如果Remainder零，则 执行PickedUp操作（销毁 Owner 道具Actor等）
	if (Result.Remainder == 0)
	{
		ItemComponent->OnPickedUp();
	}
	// 否则，更新想要捡起的Item的StackCount
	else if (FItemFragment_Stackable* StackableFragment =
		ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FItemFragment_Stackable>())
	{
		StackableFragment->SetStackCount(Result.Remainder);
	}
}

void UDkInventoryComponent::Server_AddNewItemWithItem_Implementation(
	UDkInventoryItem* Item, int32 StackCount, FDkInventorySlotAvailabilityResult Result)
{
	// 服务器FastArray添加Item后，回调PostReplicatedAdd来达到OnItemAdded.Broadcast(NewItem)的目的，以更新Client
	UDkInventoryItem* NewItem = InventoryList.AddEntry(Item);
	NewItem->SetTotalStackCount(StackCount);
	Result.Item = NewItem;

	if (OwningCharacter.IsValid())
	{
		if (OwningCharacter->GetController()->GetNetMode() == NM_ListenServer ||
			OwningCharacter->GetController()->GetNetMode() == NM_Standalone)
		{
			UpdateInventoryCategoryItemsArray(Result);
		}
	}

	if (Result.Remainder != 0)
	{
		Debug::Print(TEXT("进入背包时，剩余数量不为0，请检查！"));
	}
}

void UDkInventoryComponent::Server_AddStacksToItem_Implementation(
	UDkItemComponent* ItemComponent, FDkInventorySlotAvailabilityResult Result)
{
	const FGameplayTag& ItemTag =
		IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemTag() : FGameplayTag::EmptyTag;
	UDkInventoryItem* Item = InventoryList.FindFirstItemByTag(ItemTag);
	if (!IsValid(Item)) return;

	Item->SetTotalStackCount(Item->GetTotalStackCount() + Result.TotalRoomToFill);

	if (OwningCharacter.IsValid())
	{
		if (OwningCharacter->GetController()->GetNetMode() == NM_ListenServer ||
			OwningCharacter->GetController()->GetNetMode() == NM_Standalone)
		{
			UpdateInventoryCategoryItemsArray(Result);
		}
	}

	// 如果Remainder零，则 执行PickedUp操作（销毁 Owner 道具Actor等）
	if (Result.Remainder == 0)
	{
		ItemComponent->OnPickedUp();
	}
	// 否则，更新想要捡起的Item的StackCount
	else if (FItemFragment_Stackable* StackableFragment =
		ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FItemFragment_Stackable>())
	{
		StackableFragment->SetStackCount(Result.Remainder);
	}
}

void UDkInventoryComponent::Server_AddStacksToItemWithItem_Implementation(
	UDkInventoryItem* InItem, FDkInventorySlotAvailabilityResult Result)
{
	const FGameplayTag& ItemTag = IsValid(InItem) ? InItem->GetItemManifest().GetItemTag() : FGameplayTag::EmptyTag;
	UDkInventoryItem* Item = InventoryList.FindFirstItemByTag(ItemTag);
	if (!IsValid(Item)) return;

	Item->SetTotalStackCount(Item->GetTotalStackCount() + Result.TotalRoomToFill);

	if (OwningCharacter.IsValid())
	{
		if (OwningCharacter->GetController()->GetNetMode() == NM_ListenServer ||
			OwningCharacter->GetController()->GetNetMode() == NM_Standalone)
		{
			UpdateInventoryCategoryItemsArray(Result);
		}
	}

	if (Result.Remainder != 0)
	{
		Debug::Print(TEXT("进入背包时，剩余数量不为0，请检查！"));
		// TODO: 这里应该是没写完的，后续可以补 
	}
}

void UDkInventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
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
	if (FItemFragment_Stackable* StackableFragment =
		ItemManifest.GetFragmentOfTypeMutable<FItemFragment_Stackable>())
	{
		StackableFragment->SetStackCount(DroppedCount);
	}
	ItemManifest.SpawnPickUpActor(this, SpawnLocation, SpawnRotation);
}

void UDkInventoryComponent::ServerConsumeItem_Implementation(UDkInventoryItem* Item)
{
	if (!OwnerASC.IsValid()) return;

	const int32 NewStackCount = Item->GetTotalStackCount() - 1;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	if (FInventoryItemFragment_Consumable* ConsumableFragment =
		Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FInventoryItemFragment_Consumable>())
	{
		ConsumableFragment->OnConsume(OwnerASC.Get());
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

	InitializeInventoryComponent();

	InitInventoryCategoryItemsArray();
}

void UDkInventoryComponent::InitInventoryCategoryItemsArray()
{
	// 应该只在Client端执行
	if (!OwningCharacter.IsValid()) return;
	if (!OwningCharacter->HasAuthority()) return;

	InventoryCategoryItemsArray = FInstancedStruct::Make<FInventoryCategoryItemsArray>();

	for (const FInventoryItemCategoryInfo& CategoryInfo : CategoryInfoList)
	{
		EInventoryItemCategory ItemCategory = CategoryInfo.Category;
		if (!GetInventoryCategoryItemsArray().ContainCategory(ItemCategory))
		{
			FInventoryCategoryItemsArray& ItemsArrayMutable = GetInventoryCategoryItemsArrayMutable();
			ItemsArrayMutable.AddNewCategory(ItemCategory, CategoryInfo.Rows * CategoryInfo.Columns);
		}
	}
}

void UDkInventoryComponent::UpdateInventoryCategoryItemsArray(const FDkInventorySlotAvailabilityResult& Result)
{
	if (!Result.Item.IsValid()) return;
	EInventoryItemCategory ItemCategory = Result.Item->GetItemManifest().GetItemCategory();
	if (!GetInventoryCategoryItemsArray().ContainCategory(ItemCategory)) return;

	FInventoryCategoryItemsArray& ItemsArrayMutable = GetInventoryCategoryItemsArrayMutable();
	TArray<FInventoryItemBriefInfo>* ItemBriefInfos = ItemsArrayMutable.FindItems(ItemCategory);
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		FInventoryItemBriefInfo& ItemInfo = (*ItemBriefInfos)[Availability.Index];
		if (!Availability.bItemAtIndex)
		{
			ItemInfo.InventoryItem = Result.Item.Get();
		}
		ItemInfo.StackCount += Availability.AmountToFill;
	}

	Debug::Print("123");
	// TODO: 这边加一个OnInventoryItemBriefMapUpdated, 并把OnStackChange删掉 （用来实时更新背包的内容）
	// TODO: 这个更新InventoryItemBriefMap的也可以改成Server端执行
}

void UDkInventoryComponent::OnRep_InventoryCategoryItemsArray()
{
	
}

const TArray<FInventoryItemBriefInfo>* UDkInventoryComponent::GetItemBriefInfoByCategory(
	EInventoryItemCategory InCategory) const
{
	return GetInventoryCategoryItemsArray().FindItems(InCategory);
}

FDkInventorySlotAvailabilityResult UDkInventoryComponent::HasRoomForItem(const UDkItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FDkInventorySlotAvailabilityResult UDkInventoryComponent::HasRoomForItem(const UDkInventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FDkInventorySlotAvailabilityResult UDkInventoryComponent::HasRoomForItem(const FInventoryItemManifest& Manifest)
{
	FDkInventorySlotAvailabilityResult Result;
	EInventoryItemCategory ItemCategory = Manifest.GetItemCategory();
	if (!GetInventoryCategoryItemsArray().ContainCategory(ItemCategory)) return Result;
	// 判断物品是否可堆叠
	const FItemFragment_Stackable* StackableFragment =
		Manifest.GetFragmentOfType<FItemFragment_Stackable>();
	Result.bStackable = StackableFragment != nullptr;
	// 确定需要添加多少StackCount。AmountToFill
	const int32 MaxStackCount = StackableFragment ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = StackableFragment ? StackableFragment->GetStackCount() : 1;

	// For each Grid Slot:
	for (int iIdx = 0; iIdx < GetInventoryCategoryItemsArray().GetCategoryMaxSize(ItemCategory); ++iIdx)
	{
		const TArray<FInventoryItemBriefInfo>* ItemBriefList = GetInventoryCategoryItemsArray().FindItems(ItemCategory);
		const FInventoryItemBriefInfo& ItemBriefInfo = (*ItemBriefList)[iIdx];
		//   如果已经没有剩余要填充的数量，提前跳出循环。
		if (AmountToFill == 0) break;

		//   物品能否放进这里？
		UDkInventoryItem* CurSlotItem = ItemBriefInfo.InventoryItem;
		if (IsValid(CurSlotItem))
		{
			//     是可堆叠物品吗？如果不是，则当前索引的格子没有空间可以放Item
			if (!CurSlotItem->IsItemStackable()) continue;
			//     该物品与待添加物品Tag相同吗？
			if (!CurSlotItem->DoesItemTagMatch(Manifest.GetItemTag())) continue;
			//     如果可堆叠，该槽位是否已达到最大堆叠上限？
			if (ItemBriefInfo.StackCount >= MaxStackCount) continue;
		}

		//   需要填充多少？如果是0，说明这个格子不需要考虑
		const int32 AmountToFillInSlot =
			CalculateFillAmountForSlot(Result.bStackable, MaxStackCount, AmountToFill, ItemBriefInfo);
		if (AmountToFillInSlot == 0) continue;

		//   更新SlotAvailabilityResult
		Result.TotalRoomToFill += AmountToFillInSlot;
		Result.SlotAvailabilities.Emplace(
			ItemBriefInfo.Index,
			Result.bStackable ? AmountToFillInSlot : 0,
			IsValid(ItemBriefInfo.InventoryItem)
		);
		//   更新剩余待填充数量（AmountToFill）
		AmountToFill -= AmountToFillInSlot;
		// 剩余量是多少？
		Result.Remainder = AmountToFill;
	}

	return Result;
}

int32 UDkInventoryComponent::CalculateFillAmountForSlot(
	const bool bStackable,
	const int32 MaxStackSize,
	const int32 AmountToFill,
	const FInventoryItemBriefInfo& ItemBriefInfo) const
{
	const int32 RoomInSlot = MaxStackSize - ItemBriefInfo.StackCount;
	return bStackable ? FMath::Min(AmountToFill, RoomInSlot) : 1;
}
