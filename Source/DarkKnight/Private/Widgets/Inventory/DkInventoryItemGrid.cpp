// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryItemGrid.h"

#include "Components/DkInventoryComponent.h"
#include "Components/DkItemComponent.h"
#include "Widgets/Inventory/DkInventoryGridSlot.h"

#include "Components/UniformGridPanel.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Inventory/DkInventorySlotAvailabilty.h"
#include "Widgets/Inventory/DkInventoryDraggedItem.h"
#include "Widgets/Inventory/DkInventoryPopUpMenu.h"

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

void UDkInventoryItemGrid::PutDownOnIndex(const int32 Index)
{
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
	InventoryComponent->OnStackChange.AddDynamic(this, &ThisClass::HandleStackChanged);
	// 绑定DraggedItem创建相关的回调
	InventoryComponent->OnDraggedItemCreated.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemCreated);
	InventoryComponent->OnDraggedItemRemoved.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemRemoved);
	InventoryComponent->OnExitGameMenuRecoverGridItem.AddUniqueDynamic(
		this, &ThisClass::HandleDraggedItemRecovered
	);
}

void UDkInventoryItemGrid::AddItem(UDkInventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	FDkInventorySlotAvailabilityResult AvailabilityResult = HasRoomForItem(Item);
	AddItemToIndices(AvailabilityResult, Item);
}

void UDkInventoryItemGrid::HandleStackChanged(const FDkInventorySlotAvailabilityResult& Result)
{
	AddStacks(Result);
}

void UDkInventoryItemGrid::AddStacks(const FDkInventorySlotAvailabilityResult& Result)
{
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
	GridSlot->SetUnoccupiedBrush();
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

bool UDkInventoryItemGrid::IsSameStackableWithDraggedItem(const UDkInventoryItem* ClickedInventoryItem)
{
	const bool bIsSameItem = ClickedInventoryItem == DraggedItem->GetInventoryItem();
	const bool bIsStackable = ClickedInventoryItem->IsItemStackable();
	return bIsSameItem && bIsStackable &&
		DraggedItem->GetItemTag().MatchesTagExact(ClickedInventoryItem->GetItemManifest().GetItemTag());
}

void UDkInventoryItemGrid::SwapWithDraggedItem(UDkInventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
}

void UDkInventoryItemGrid::ConsumeDraggedItemStack(int32 ClickedStackCount, int32 DraggedStackCount, int32 GridIndex)
{
}

void UDkInventoryItemGrid::DragItem(UDkInventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	AssignDraggedItem(ClickedInventoryItem, GridIndex, GridIndex);

	// 从背包中移除被点击的Item
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
}

void UDkInventoryItemGrid::AssignDraggedItem(UDkInventoryItem* InventoryItem)
{
}

void UDkInventoryItemGrid::AssignDraggedItem(
	UDkInventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex)
{
	AssignDraggedItem(InventoryItem);

	DraggedItem->SetPreviousGridIndex(PreviousGridIndex);
	DraggedItem->SetIsPreviousEquipped(false);
	DraggedItem->UpdateStackCount(InventoryItem->IsItemStackable() ? GridSlots[GridIndex]->GetStackCount() : 0);
}

void UDkInventoryItemGrid::RemoveItemFromGrid(UDkInventoryItem* InventoryItem, const int32 GridIndex)
{
}

void UDkInventoryItemGrid::ClearDraggedItem()
{
	if (!IsValid(DraggedItem)) return;

	DraggedItem->SetInventoryItem(nullptr);
	DraggedItem->SetIsStackable(false);
	DraggedItem->SetPreviousGridIndex(INDEX_NONE);
	DraggedItem->UpdateStackCount(0);
	DraggedItem->SetImageBrush(FSlateNoResource());

	check(InventoryComponent.IsValid());
	if (IsValid(DraggedItem))
	{
		DraggedItem->RemoveFromParent();
	}
	DraggedItem = nullptr;
	InventoryComponent->OnDraggedItemRemoved.Broadcast();
}

void UDkInventoryItemGrid::HandleDraggedItemCreated(UDkInventoryDraggedItem* InDraggedItem)
{
	if (!IsValid(InDraggedItem)) return;

	if (IsValid(DraggedItem))
	{
		DraggedItem = nullptr;
	}
	DraggedItem = InDraggedItem;
	DraggedItem->OnDraggedItemClicked.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemClicked);
}

void UDkInventoryItemGrid::HandleDraggedItemRecovered(UDkInventoryDraggedItem* InDraggedItem)
{
	if (!IsValid(InDraggedItem->GetInventoryItem()) || !MatchesCategory(InDraggedItem->GetInventoryItem())) return;

	if (!InDraggedItem->IsPreviousEquipped())
	{
		if (IsValid(InDraggedItem->GetInventoryItem()))
		{
			PutDownOnIndex(InDraggedItem->GetPreviousGridIndex());
		}
	}
}

void UDkInventoryItemGrid::HandleDraggedItemRemoved()
{
	DraggedItem = nullptr;
}

void UDkInventoryItemGrid::HandleDraggedItemClicked(const FPointerEvent& MouseEvent)
{
	OnDraggedItemClicked(MouseEvent);
}

void UDkInventoryItemGrid::OnDraggedItemClicked(const FPointerEvent& MouseEvent)
{
}

void UDkInventoryItemGrid::CreateItemPopUp(const int32 GridIndex)
{
	UDkInventoryItem* RightClickedItem = GridSlots[GridIndex]->GetInventoryItem();
	if (!IsValid(RightClickedItem)) return;

	// 设置PopUpMenu弹出位置
	PopUpMenu = CreateWidget<UDkInventoryPopUpMenu>(this, PopUpMenuClass);
	if (IsValid(PopUpMenu))
	{
		PopUpMenu->SetGridIndex(GridIndex);
		PopUpMenu->AddToViewport();
	}
	if (PopUpMenu && PopUpMenu->IsInViewport())
	{
		FVector2D MousePos;
		if (UGameViewportClient* VP = GetWorld()->GetGameViewport())
		{
			VP->GetMousePosition(MousePos); // 系统硬件像素（受 DPI 缩放）
		}

		PopUpMenu->SetPositionInViewport(MousePos - FVector2D(10, 10));
	}

	// 设置PopUpMenu内容
	const int32 SliderMax = GridSlots[GridIndex]->GetStackCount() - 1;
	if (RightClickedItem->IsItemStackable() && SliderMax > 0)
	{
		PopUpMenu->OnSplit.BindDynamic(this, &ThisClass::HandlePopUpMenuSplit);
		PopUpMenu->SetSliderParams(SliderMax, FMath::Max(1, GridSlots[GridIndex]->GetStackCount() / 2));
	}
	else
	{
		PopUpMenu->CollapseSplitButton();
	}

	PopUpMenu->OnDrop.BindDynamic(this, &ThisClass::OnPopUpMenuDrop);

	if (RightClickedItem->GetItemManifest().GetItemCategory() == EInventoryItemCategory::Consumable)
	{
		PopUpMenu->OnConsume.BindDynamic(this, &ThisClass::HandlePopUpMenuConsume);
	}
	else
	{
		PopUpMenu->CollapseConsumeButton();
	}
}

void UDkInventoryItemGrid::HandlePopUpMenuSplit(int32 SplitAmount, int32 Index)
{
	OnPopUpMenuSplit(SplitAmount, Index);
}

void UDkInventoryItemGrid::OnPopUpMenuSplit(int32 SplitAmount, int32 Index)
{
}

void UDkInventoryItemGrid::OnPopUpMenuDrop(int32 Index)
{
	UDkInventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem();
	if (!IsValid(RightClickedItem)) return;

	DragItem(RightClickedItem, Index);
	DropItem();
}

void UDkInventoryItemGrid::DropItem()
{
	if (!IsValid(DraggedItem)) return;
	if (!IsValid(DraggedItem->GetInventoryItem())) return;

	InventoryComponent->ServerDropItem(DraggedItem->GetInventoryItem(), DraggedItem->GetStackCount());

	ClearDraggedItem();
}

void UDkInventoryItemGrid::HandlePopUpMenuConsume(int32 Index)
{
	OnPopUpMenuConsume(Index);
}

void UDkInventoryItemGrid::OnPopUpMenuConsume(int32 Index)
{
}
