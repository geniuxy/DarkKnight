// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryItemGrid.h"

#include "Components/DkInventoryComponent.h"
#include "Components/DkItemComponent.h"
#include "Widgets/Inventory/DkInventoryGridSlot.h"

#include "Components/UniformGridPanel.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Inventory/DkInventorySlotAvailabilty.h"
#include "Widgets/Inventory/DkInventoryDraggedItem.h"
#include "Widgets/Inventory/DkInventorySlottedItem.h"

FDkInventorySlotAvailabilityResult UDkInventoryItemGrid::HasRoomForItem(const UDkItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FDkInventorySlotAvailabilityResult UDkInventoryItemGrid::HasRoomForItem(const UDkInventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FDkInventorySlotAvailabilityResult UDkInventoryItemGrid::HasRoomForItem(const FInventoryItemManifest& Manifest)
{
	return FDkInventorySlotAvailabilityResult();
}

void UDkInventoryItemGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GridPanel)
	{
		GridPanel->SetSlotPadding(FMargin(SlotDistance));
	}

	ConstructGrid();

	InventoryComponent = UDkInventoryFunctionLibrary::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
	InventoryComponent->OnStackChange.AddDynamic(this, &ThisClass::AddStacks);
}

void UDkInventoryItemGrid::AddItem(UDkInventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	FDkInventorySlotAvailabilityResult AvailabilityResult = HasRoomForItem(Item);
	AddItemToIndices(AvailabilityResult, Item);
}

void UDkInventoryItemGrid::AddStacks(const FDkInventorySlotAvailabilityResult& Result)
{
	if (!Result.Item.IsValid() || !MatchesCategory(Result.Item.Get())) return;

	for (const auto& Availability : Result.SlotAvailabilities)
	{
		if (Availability.bItemAtIndex)
		{
			const TObjectPtr<UDkInventoryGridSlot>& GridSlot = GridSlots[Availability.Index];
			const TObjectPtr<UDkInventorySlottedItem>& SlottedItem = SlottedItemMap.FindChecked(Availability.Index);
			SlottedItem->UpdateStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
			GridSlot->SetStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
		}
		else
		{
			AddItemToIndex(Result.Item.Get(), Availability.Index, Availability.AmountToFill, Result.bStackable);
			UpdateGridSlots(Result.Item.Get(), Availability.Index, Availability.AmountToFill, Result.bStackable);
		}
	}
}

void UDkInventoryItemGrid::AddItemToIndices(const FDkInventorySlotAvailabilityResult& Result, UDkInventoryItem* NewItem)
{
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		AddItemToIndex(NewItem, Availability.Index, Availability.AmountToFill, Result.bStackable);
		UpdateGridSlots(NewItem, Availability.Index, Availability.AmountToFill, Result.bStackable);
	}
}

void UDkInventoryItemGrid::AddItemToIndex(UDkInventoryItem* NewItem, int32 Index, int32 StackAmount, bool bStackable)
{
}

bool UDkInventoryItemGrid::HasRoomAtIndex(
	const UDkInventoryGridSlot* CurIndexGridSlot,
	const FIntPoint& Dimension,
	const TSet<int32>& CheckedIndices,
	TSet<int32>& OutTemporarilyClaimed,
	const FGameplayTag& ItemTag,
	const int32 MaxStackCount)
{
	//   该索引处是否有空位？（是否有其他物品阻挡?）
	bool bHasRoomAtIndex = true;

	//   检查其它重要条件——在二维范围内做 ForEach2D
	UDkInventoryFunctionLibrary::ForEach2D(
		GridSlots, CurIndexGridSlot->GetTileIndex(), Dimension, Columns,
		[&](const UDkInventoryGridSlot* SubGridSlot)
		{
			if (CheckSlotConstraints(CurIndexGridSlot, SubGridSlot, CheckedIndices, OutTemporarilyClaimed,
			                         ItemTag, MaxStackCount))
			{
				OutTemporarilyClaimed.Add(SubGridSlot->GetTileIndex());
			}
			else
			{
				bHasRoomAtIndex = false;
			}
		}
	);

	return bHasRoomAtIndex;
}

bool UDkInventoryItemGrid::CheckSlotConstraints(
	const UDkInventoryGridSlot* CurIndexGridSlot,
	const UDkInventoryGridSlot* SubGridSlot,
	const TSet<int32>& CheckedIndices,
	TSet<int32>& OutTemporarilyClaimed,
	const FGameplayTag& ItemTag,
	const int32 MaxStackCount) const
{
	//     子GridSlot是否已被检查过？
	if (CheckedIndices.Contains(SubGridSlot->GetTileIndex())) return false;
	//     子GridSlot是否有有效物品？如果没有，则当前子GridSlot可能有空间可以放Item
	UDkInventoryItem* SubItem = SubGridSlot->GetInventoryItem();
	if (!IsValid(SubItem))
	{
		OutTemporarilyClaimed.Add(SubGridSlot->GetTileIndex());
		return true;
	}
	// 如果子GridSlot有有效的Item，
	//	   子GridSlot的左上角Slot是当前索引的GridSlot吗？如果不是，则当前索引的格子没有空间可以放Item
	if (SubGridSlot->GetUpperLeftIndex() != CurIndexGridSlot->GetTileIndex()) return false;
	//     是可堆叠物品吗？如果不是，则当前索引的格子没有空间可以放Item
	if (!SubItem->IsItemStackable()) return false;
	//     该物品与待添加物品Tag相同吗？
	if (!SubItem->DoesItemTagMatch(ItemTag)) return false;
	//     如果可堆叠，该槽位是否已达到最大堆叠上限？
	if (CurIndexGridSlot->GetStackCount() >= MaxStackCount) return false;

	return true;
}

bool UDkInventoryItemGrid::MatchesCategory(const UDkInventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}

void UDkInventoryItemGrid::UpdateGridSlots(
	UDkInventoryItem* NewItem, const int32 Index, int32 StackAmount, bool bStackable)
{
	check(GridSlots.IsValidIndex(Index));

	UDkInventoryGridSlot* GridSlot = GridSlots[Index];
	GridSlot->SetOccupiedTexture();
}

void UDkInventoryItemGrid::ConstructGrid()
{
	GridSlots.Reset();
	GridSlots.Reserve(Rows * Columns);

	for (int32 j = 0; j < Rows; ++j)
	{
		for (int32 i = 0; i < Columns; ++i)
		{
			UDkInventoryGridSlot* GridSlot = CreateWidget<UDkInventoryGridSlot>(this, GridSlotClass);
			GridPanel->AddChildToUniformGrid(GridSlot, j, i);

			GridSlot->SetGridSlotSize(TileSize);
			GridSlot->SetTileIndex(UDkInventoryFunctionLibrary::GetIndexFromPosition({i, j}, Columns));

			GridSlots.Add(GridSlot);
		}
	}
}

void UDkInventoryItemGrid::OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	check(GridSlots.IsValidIndex(GridIndex));
	UDkInventoryItem* ClickedInventoryItem = GridSlots[GridIndex]->GetInventoryItem();
	if (!IsValid(DraggedItem) && IsLeftMouseClick(MouseEvent))
	{
		// TODO: 拖拽Item
	}
}

bool UDkInventoryItemGrid::IsRightMouseClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UDkInventoryItemGrid::IsLeftMouseClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}
