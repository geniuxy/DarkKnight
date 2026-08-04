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
		// OnStackChange.Broadcast(AddItemResult); // OnStackChange在Server和Client都会执行，光在Server执行，无法同步到Client
		Server_AddStacksToItem(ItemComponent, AddItemResult, bNeedNotice);
	}
	else
	{
		int StackCount = AddItemResult.bStackable ? AddItemResult.TotalRoomToFill : 0;
		// 此物品类型在物品栏中不存在。请创建一个新物品并更新所有相关栏位。
		Server_AddNewItem(ItemComponent, StackCount, AddItemResult, bNeedNotice);
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
		// 为背包中已存在的物品添加堆叠数量。我们只想更新堆叠数量，
		// 而不是创建这种类型的新物品。
		// OnStackChange.Broadcast(AddItemResult); // OnStackChange在Server和Client都会执行，光在Server执行，无法同步到Client
		Server_AddStacksToItemWithItem(Item, AddItemResult, bNeedNotice);
	}
	else
	{
		int StackCount = AddItemResult.bStackable ? AddItemResult.TotalRoomToFill : 0;
		// 此物品类型在物品栏中不存在。请创建一个新物品并更新所有相关栏位。
		Server_AddNewItemWithItem(Item, StackCount, AddItemResult, bNeedNotice);
	}
}

void UDkInventoryComponent::Server_AddNewItem_Implementation(
	UDkItemComponent* ItemComponent, int32 StackCount, FDkInventorySlotAvailabilityResult Result, bool bNeedNotice)
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

	int Count = Result.bStackable ? StackCount : 1;
	Client_ShowItemNotice(ItemComponent->GetItemName(), Count, bNeedNotice);
}

void UDkInventoryComponent::Server_AddNewItemWithItem_Implementation(
	UDkInventoryItem* Item, int32 StackCount, FDkInventorySlotAvailabilityResult Result, bool bNeedNotice)
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

	int Count = Result.bStackable ? StackCount : 1;
	Client_ShowItemNotice(Item->GetItemName(), Count, bNeedNotice);
}

void UDkInventoryComponent::Server_AddStacksToItem_Implementation(
	UDkItemComponent* ItemComponent, FDkInventorySlotAvailabilityResult Result, bool bNeedNotice)
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

	Client_ShowItemNotice(ItemComponent->GetItemName(), Result.TotalRoomToFill, bNeedNotice);
}

void UDkInventoryComponent::Server_AddStacksToItemWithItem_Implementation(
	UDkInventoryItem* InItem, FDkInventorySlotAvailabilityResult Result, bool bNeedNotice)
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

	Client_ShowItemNotice(Item->GetItemName(), Result.TotalRoomToFill, bNeedNotice);
}

void UDkInventoryComponent::Client_ShowItemNotice_Implementation(
	const FText& ItemName, int32 Count, bool bNeedNotice)
{
	if (bNeedNotice)
	{
		OnAddItemNotice.Broadcast(ItemName, Count);
	}
}

void UDkInventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

void UDkInventoryComponent::ServerDropItem_Implementation(
	UDkInventoryItem* Item, EInventoryItemCategory Category, int32 Index, int32 StackCount)
{
	if (!Item) return;

	const int32 NewStackCount = Item->GetTotalStackCount() - StackCount;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	FDkInventorySlotEntry* SlotEntry =
		InventorySlotArray.FindBySlotIndex(Category, Index);
	if (SlotEntry->BriefInfo.InventoryItem &&
		SlotEntry->BriefInfo.InventoryItem->GetItemTag() == Item->GetItemTag())
	{
		if (SlotEntry->BriefInfo.StackCount <= StackCount)
		{
			SlotEntry->BriefInfo.InventoryItem = nullptr;
		}
		SlotEntry->BriefInfo.StackCount -= StackCount;
		InventorySlotArray.MarkItemDirty(*SlotEntry);
	}

	SpawnDroppedItem(Item, StackCount);
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
	if (IsValid(EquippedItem))
	{
		OnItemEquipped.Broadcast(EquippedItem);
	}

	if (IsValid(UnEquippedItem))
	{
		OnItemUnEquipped.Broadcast(UnEquippedItem);
	}

	ClientUpdateEquippedItem(EquippedItem, UnEquippedItem);
}

void UDkInventoryComponent::ClientUpdateEquippedItem_Implementation(
	UDkInventoryItem* EquippedItem, UDkInventoryItem* UnEquippedItem)
{
	// 目前主要用来更新Client端的PreviewActor的装备
	if (OwningCharacter->HasAuthority()) return;
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

	FTimerHandle InitTimeHandle;
	GetWorld()->GetTimerManager().SetTimer(InitTimeHandle, FTimerDelegate::CreateLambda([this]()
	{
		InitializeInventoryComponent();
		InitInventorySlotArray();
	}), 0.2f, false);
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
		UDkInventoryItem* ItemInBag = ItemBriefInfos[SlotEntry->BriefInfo.Index].InventoryItem;
		UDkInventoryItem* ItemInComp = SlotEntry->BriefInfo.InventoryItem;
		int ItemStackCountInBag = ItemBriefInfos[SlotEntry->BriefInfo.Index].StackCount;
		int ItemStackCountInComp = SlotEntry->BriefInfo.StackCount;
		if (ItemStackCountInComp != ItemStackCountInBag || ItemInComp != ItemInBag)
		{
			SlotEntry->BriefInfo.InventoryItem = ItemInBag;
			SlotEntry->BriefInfo.StackCount = ItemStackCountInBag;
			InventorySlotArray.MarkItemDirty(*SlotEntry);
		}
	}
}

bool UDkInventoryComponent::Server_UpdateInventorySlotArray_Validate(
	EInventoryItemCategory InCategory, const TArray<FInventoryItemBriefInfo>& ItemBriefInfos)
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

void UDkInventoryComponent::RemoveItem(EInventoryItemCategory InCategory, int Index, int Count)
{
	FDkInventorySlotEntry* SlotEntry = InventorySlotArray.FindBySlotIndex(InCategory, Index);
	if (SlotEntry->BriefInfo.StackCount <= Count)
	{
		if (SlotEntry->BriefInfo.InventoryItem->GetTotalStackCount() <= Count)
		{
			InventoryList.RemoveEntry(SlotEntry->BriefInfo.InventoryItem);
		}
		SlotEntry->BriefInfo.InventoryItem = nullptr;
	}
	else
	{
		SlotEntry->BriefInfo.InventoryItem->SetTotalStackCount(
			SlotEntry->BriefInfo.InventoryItem->GetTotalStackCount() - Count
		);
	}
	SlotEntry->BriefInfo.StackCount = FMath::Max(SlotEntry->BriefInfo.StackCount - Count, 0);
	InventorySlotArray.MarkItemDirty(*SlotEntry);
}

void UDkInventoryComponent::TryToRemoveItem(EInventoryItemCategory InCategory, int Index, int Count)
{
	Server_RemoveItem(InCategory, Index, Count);
}

void UDkInventoryComponent::Server_RemoveItem_Implementation(EInventoryItemCategory InCategory, int Index, int Count)
{
	RemoveItem(InCategory, Index, Count);
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
