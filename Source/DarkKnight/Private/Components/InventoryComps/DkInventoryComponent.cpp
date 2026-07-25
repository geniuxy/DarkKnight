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
#include "Widgets/Inventory/DkInventoryDraggedItem.h"

UDkInventoryComponent::UDkInventoryComponent() : InventoryList(this), InventorySlotArray(this)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

void UDkInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
	DOREPLIFETIME(ThisClass, InventorySlotArray);
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

void UDkInventoryComponent::TryAddItem(UDkItemComponent* ItemComponent, bool bNeedNotice)
{
	// TODO:为什么计算的工作放到客户端做..
	FDkInventorySlotAvailabilityResult AddItemResult = HasRoomForItem(ItemComponent);

	UDkInventoryItem* FoundItem = InventoryList.FindFirstItemByTag(ItemComponent->GetItemManifest().GetItemTag());
	AddItemResult.Item = FoundItem;

	if (AddItemResult.TotalRoomToFill == 0)
	{
		OnNoRoomInInventory.Broadcast(FText::FromString(TEXT("背包中没有足够的空间了")));
		return;
	}

	// 将Item添加到Inventory中
	if (IsValid(AddItemResult.Item) && AddItemResult.bStackable)
	{
		// 为背包中已存在的物品添加堆叠数量。我们只想更新堆叠数量，
		// 而不是创建这种类型的新物品。
		if (bNeedNotice)
		{
			OnAddItemNotice.Broadcast(ItemComponent->GetItemName(), AddItemResult.TotalRoomToFill);
		}
		// OnStackChange.Broadcast(AddItemResult); // OnStackChange在Server和Client都会执行，光在Server执行，无法同步到Client
		Server_AddStacksToItem(ItemComponent, AddItemResult);
	}
	else
	{
		int StackCount = AddItemResult.bStackable ? AddItemResult.TotalRoomToFill : 0;
		if (bNeedNotice)
		{
			OnAddItemNotice.Broadcast(
				ItemComponent->GetItemName(),
				AddItemResult.bStackable ? StackCount : 1
			);
		}
		// 此物品类型在物品栏中不存在。请创建一个新物品并更新所有相关栏位。
		Server_AddNewItem(ItemComponent, StackCount, AddItemResult);
	}
}

void UDkInventoryComponent::TryAddItem(UDkInventoryItem* Item, bool bNeedNotice)
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
	if (IsValid(AddItemResult.Item) && AddItemResult.bStackable)
	{
		if (bNeedNotice)
		{
			OnAddItemNotice.Broadcast(Item->GetItemName(), AddItemResult.TotalRoomToFill);
		}
		// 为背包中已存在的物品添加堆叠数量。我们只想更新堆叠数量，
		// 而不是创建这种类型的新物品。
		// OnStackChange.Broadcast(AddItemResult); // OnStackChange在Server和Client都会执行，光在Server执行，无法同步到Client
		Server_AddStacksToItemWithItem(Item, AddItemResult);
	}
	else
	{
		int StackCount = AddItemResult.bStackable ? AddItemResult.TotalRoomToFill : 0;
		if (bNeedNotice)
		{
			OnAddItemNotice.Broadcast(
				Item->GetItemName(),
				AddItemResult.bStackable ? StackCount : 1
			);
		}
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

	UpdateInventorySlotArray(Result);

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

	UpdateInventorySlotArray(Result);

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

	UpdateInventorySlotArray(Result);

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

	UpdateInventorySlotArray(Result);

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

void UDkInventoryComponent::ServerDropItem_Implementation(UDkInventoryDraggedItem* DroppedItem)
{
	if (!DroppedItem || !DroppedItem->GetInventoryItem()) return;

	const int32 NewStackCount = DroppedItem->GetInventoryItem()->GetTotalStackCount() - DroppedItem->GetStackCount();
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(DroppedItem->GetInventoryItem());
	}
	else
	{
		DroppedItem->GetInventoryItem()->SetTotalStackCount(NewStackCount);
	}

	FDkInventorySlotEntry* SlotEntry =
		InventorySlotArray.FindBySlotIndex(DroppedItem->GetItemCategory(), DroppedItem->GetPreviousGridIndex());
	if (SlotEntry->BriefInfo.InventoryItem &&
		SlotEntry->BriefInfo.InventoryItem->GetItemTag() == DroppedItem->GetItemTag())
	{
		if (SlotEntry->BriefInfo.StackCount <= DroppedItem->GetStackCount())
		{
			SlotEntry->BriefInfo.InventoryItem = nullptr;
		}
		SlotEntry->BriefInfo.StackCount -= DroppedItem->GetStackCount();
		InventorySlotArray.MarkItemDirty(*SlotEntry);
	}

	SpawnDroppedItem(DroppedItem->GetInventoryItem(), DroppedItem->GetStackCount());
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

void UDkInventoryComponent::ServerConsumeItem_Implementation(UDkInventoryItem* Item, int Index)
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

	FDkInventorySlotEntry* SlotEntry = InventorySlotArray.FindBySlotIndex(Item->GetItemCategory(), Index);
	if (SlotEntry->BriefInfo.StackCount <= 1)
	{
		SlotEntry->BriefInfo.InventoryItem = nullptr;
	}
	SlotEntry->BriefInfo.StackCount -= 1;
	InventorySlotArray.MarkItemDirty(*SlotEntry);

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

	InitInventorySlotArray();
}

void UDkInventoryComponent::InitInventorySlotArray()
{
	if (!OwningCharacter.IsValid()) return;
	if (!OwningCharacter->HasAuthority()) return;

	int GlobalIndex = 0;
	for (const FInventoryItemCategoryInfo& CategoryInfo : CategoryInfoList)
	{
		for (int iSlotIndex = 0; iSlotIndex < CategoryInfo.Rows * CategoryInfo.Columns; iSlotIndex++)
		{
			FDkInventorySlotEntry& NewEntry = InventorySlotArray.Slots.AddDefaulted_GetRef();
			NewEntry.Category = CategoryInfo.Category;
			NewEntry.GlobalIndex = GlobalIndex;
			NewEntry.BriefInfo = FInventoryItemBriefInfo(iSlotIndex);
			GlobalIndex++;
		}
	}

	InventorySlotArray.MarkArrayDirty();
}

void UDkInventoryComponent::UpdateInventorySlotArray(const FDkInventorySlotAvailabilityResult& Result)
{
	if (!IsValid(Result.Item)) return;
	EInventoryItemCategory ItemCategory = Result.Item->GetItemManifest().GetItemCategory();
	if (!HasCategory(ItemCategory)) return;

	for (const auto& Availability : Result.SlotAvailabilities)
	{
		FDkInventorySlotEntry* SlotEntry = InventorySlotArray.FindBySlotIndex(ItemCategory, Availability.Index);
		if (!Availability.bItemAtIndex)
		{
			SlotEntry->BriefInfo.InventoryItem = Result.Item;
		}
		SlotEntry->BriefInfo.StackCount += Availability.AmountToFill;
		InventorySlotArray.MarkItemDirty(*SlotEntry);
	}

	Debug::Print("123");
	// TODO: 这边加一个OnInventoryItemBriefMapUpdated, 并把OnStackChange删掉 （用来实时更新背包的内容）
	// TODO: 这个更新InventoryItemBriefMap的也可以改成Server端执行
}

void UDkInventoryComponent::SetInventorySlotArray(
	EInventoryItemCategory InCategory, const TArray<FInventoryItemBriefInfo>& ItemBriefInfos)
{
	Server_UpdateInventorySlotArray(InCategory, ItemBriefInfos);
}

void UDkInventoryComponent::Server_UpdateInventorySlotArray_Implementation(
	EInventoryItemCategory InCategory, const TArray<FInventoryItemBriefInfo>& ItemBriefInfos)
{
	TArray<FDkInventorySlotEntry*> SlotEntries = InventorySlotArray.FindByCategory(InCategory);
	for (FDkInventorySlotEntry* SlotEntry : SlotEntries)
	{
		SlotEntry->BriefInfo.InventoryItem = ItemBriefInfos[SlotEntry->BriefInfo.Index].InventoryItem;
		SlotEntry->BriefInfo.StackCount = ItemBriefInfos[SlotEntry->BriefInfo.Index].StackCount;
		InventorySlotArray.MarkItemDirty(*SlotEntry);
	}
}

bool UDkInventoryComponent::Server_UpdateInventorySlotArray_Validate(EInventoryItemCategory InCategory,
                                                                     const TArray<FInventoryItemBriefInfo>&
                                                                     ItemBriefInfos)
{
	return true;
}

TArray<FInventoryItemBriefInfo> UDkInventoryComponent::GetCategorySlots(EInventoryItemCategory Category) const
{
	TArray<FInventoryItemBriefInfo> Result;
	for (const auto& Slot : InventorySlotArray.Slots)
	{
		if (Slot.Category == Category)
		{
			Result.Add(Slot.BriefInfo);
		}
	}
	Result.Sort([](const FInventoryItemBriefInfo& A, const FInventoryItemBriefInfo& B)
	{
		return A.Index < B.Index;
	});
	return Result;
}

void UDkInventoryComponent::TryToRemoveItem(EInventoryItemCategory InCategory, int Index, int Count)
{
	Server_RemoveItem(InCategory, Index, Count);
}

void UDkInventoryComponent::Server_RemoveItem_Implementation(EInventoryItemCategory InCategory, int Index, int Count)
{
	FDkInventorySlotEntry* SlotEntry = InventorySlotArray.FindBySlotIndex(InCategory, Index);
	if (SlotEntry->BriefInfo.StackCount <= Count)
	{
		SlotEntry->BriefInfo.InventoryItem = nullptr;
	}
	SlotEntry->BriefInfo.StackCount -= Count;
	InventorySlotArray.MarkItemDirty(*SlotEntry);
}

bool UDkInventoryComponent::Server_RemoveItem_Validate(EInventoryItemCategory InCategory, int Index, int Count)
{
	return true;
}

bool UDkInventoryComponent::HasCategory(EInventoryItemCategory InCategory)
{
	FInventoryItemCategoryInfo* ItemCategoryInfo = CategoryInfoList.FindByPredicate(
		[InCategory](const FInventoryItemCategoryInfo& CategoryInfo)
		{
			return CategoryInfo.Category == InCategory;
		}
	);
	return ItemCategoryInfo != nullptr;
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
	if (!HasCategory(ItemCategory)) return Result;
	// 判断物品是否可堆叠
	const FItemFragment_Stackable* StackableFragment =
		Manifest.GetFragmentOfType<FItemFragment_Stackable>();
	Result.bStackable = StackableFragment != nullptr;
	// 确定需要添加多少StackCount。AmountToFill
	const int32 MaxStackCount = StackableFragment ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = StackableFragment ? StackableFragment->GetStackCount() : 1;

	// For each Grid Slot:
	for (FDkInventorySlotEntry* Slot : InventorySlotArray.FindByCategory(ItemCategory))
	{
		//   如果已经没有剩余要填充的数量，提前跳出循环。
		if (AmountToFill == 0) break;

		//   物品能否放进这里？
		UDkInventoryItem* CurSlotItem = Slot->BriefInfo.InventoryItem;
		if (IsValid(CurSlotItem))
		{
			//     是可堆叠物品吗？如果不是，则当前索引的格子没有空间可以放Item
			if (!CurSlotItem->IsItemStackable()) continue;
			//     该物品与待添加物品Tag相同吗？
			if (!CurSlotItem->DoesItemTagMatch(Manifest.GetItemTag())) continue;
			//     如果可堆叠，该槽位是否已达到最大堆叠上限？
			if (Slot->BriefInfo.StackCount >= MaxStackCount) continue;
		}

		//   需要填充多少？如果是0，说明这个格子不需要考虑
		const int32 AmountToFillInSlot =
			CalculateFillAmountForSlot(Result.bStackable, MaxStackCount, AmountToFill, Slot->BriefInfo);
		if (AmountToFillInSlot == 0) continue;

		//   更新SlotAvailabilityResult
		Result.TotalRoomToFill += AmountToFillInSlot;
		Result.SlotAvailabilities.Emplace(
			Slot->BriefInfo.Index,
			Result.bStackable ? AmountToFillInSlot : 0,
			IsValid(Slot->BriefInfo.InventoryItem)
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
