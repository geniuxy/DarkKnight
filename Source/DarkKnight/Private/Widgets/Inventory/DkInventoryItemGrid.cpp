// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryItemGrid.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/InventoryComps/DkInventoryComponent.h"
#include "Components/DkItemComponent.h"
#include "Widgets/Inventory/DkInventoryGridSlot.h"

#include "Components/UniformGridPanel.h"
#include "Components/InventoryComps/DkPlayerInventoryComp.h"
#include "FunctionLibrarys/DkCommonFunctionLibrary.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Inventory/DkInventoryItemFragment.h"
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

void UDkInventoryItemGrid::SetInventoryComp(UDkInventoryComponent* InInventoryComp)
{
	InventoryComponent = InInventoryComp;
	// InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
	// InventoryComponent->OnStackChange.AddDynamic(this, &ThisClass::HandleStackChanged);
	// 绑定DraggedItem创建相关的回调
	InventoryComponent->OnDraggedItemCreated.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemCreated);
	InventoryComponent->OnDraggedItemRemoved.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemRemoved);
	InventoryComponent->OnExitGameMenuRecoverGridItem.AddUniqueDynamic(
		this, &ThisClass::HandleDraggedItemRecovered
	);
	InventoryComponent->OnInventoryCategoryItemsArrayUpdated.AddUObject(
		this, &ThisClass::HandleInventoryCategoryItemsArrayUpdated
	);
}

void UDkInventoryItemGrid::ClearItems()
{
	GridSlots.Reset();
	UniformGridPanel->ClearChildren();
}

void UDkInventoryItemGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (UniformGridPanel)
	{
		UniformGridPanel->SetSlotPadding(FMargin(SlotDistance));
	}
}

void UDkInventoryItemGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 根据鼠标的位置，更改Hover的格子样式
	const FVector2D PanelPosition = UDkUIFunctionLibrary::GetWidgetPosition(UniformGridPanel);
	// 这里的MousePosition是逻辑/虚拟像素大小，不受DPI影响
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	if (CursorExitedCanvas(PanelPosition, UDkUIFunctionLibrary::GetWidgetSize(UniformGridPanel), MousePosition))
	{
		return;
	}

	UpdateItemDropIndex(PanelPosition, MousePosition);
}

FDkInventorySlotAvailabilityResult UDkInventoryItemGrid::HasRoomForItem(const FInventoryItemManifest& Manifest)
{
	FDkInventorySlotAvailabilityResult Result;
	// 判断物品是否可堆叠
	const FItemFragment_Stackable* StackableFragment =
		Manifest.GetFragmentOfType<FItemFragment_Stackable>();
	Result.bStackable = StackableFragment != nullptr;
	// 确定需要添加多少StackCount。AmountToFill
	const int32 MaxStackCount = StackableFragment ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = StackableFragment ? StackableFragment->GetStackCount() : 1;

	// For each Grid Slot:
	for (const auto& GridSlot : GridSlots)
	{
		//   如果已经没有剩余要填充的数量，提前跳出循环。
		if (AmountToFill == 0) break;

		//   物品能否放进这里？
		UDkInventoryItem* CurSlotItem = GridSlot->GetInventoryItem();
		if (IsValid(CurSlotItem))
		{
			//     是可堆叠物品吗？如果不是，则当前索引的格子没有空间可以放Item
			if (!CurSlotItem->IsItemStackable()) continue;
			//     该物品与待添加物品Tag相同吗？
			if (!CurSlotItem->DoesItemTagMatch(Manifest.GetItemTag())) continue;
			//     如果可堆叠，该槽位是否已达到最大堆叠上限？
			if (GridSlot->GetStackCount() >= MaxStackCount) continue;
		}

		//   需要填充多少？如果是0，说明这个格子不需要考虑
		const int32 AmountToFillInSlot =
			CalculateFillAmountForSlot(Result.bStackable, MaxStackCount, AmountToFill, GridSlot);
		if (AmountToFillInSlot == 0) continue;

		//   更新SlotAvailabilityResult
		Result.TotalRoomToFill += AmountToFillInSlot;
		Result.SlotAvailabilities.Emplace(
			GridSlot->GetTileIndex(),
			Result.bStackable ? AmountToFillInSlot : 0,
			IsValid(GridSlot->GetInventoryItem())
		);
		//   更新剩余待填充数量（AmountToFill）
		AmountToFill -= AmountToFillInSlot;
		// 剩余量是多少？
		Result.Remainder = AmountToFill;
	}

	return Result;
}

int32 UDkInventoryItemGrid::CalculateFillAmountForSlot(
	const bool bStackable, const int32 MaxStackSize,
	const int32 AmountToFill, const UDkInventoryGridSlot* GridSlot) const
{
	const int32 RoomInSlot = MaxStackSize - GetStackAmount(GridSlot);
	return bStackable ? FMath::Min(AmountToFill, RoomInSlot) : 1;
}

int32 UDkInventoryItemGrid::GetStackAmount(const UDkInventoryGridSlot* GridSlot) const
{
	int32 CurrentSlotStackCount = GridSlot->GetStackCount();
	// 如果在一个并不储存数量的格子，则返回左上角格子的StackCount
	if (const int32 UpperLeftIndex = GridSlot->GetUpperLeftIndex(); UpperLeftIndex != INDEX_NONE)
	{
		UDkInventoryGridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
		CurrentSlotStackCount = UpperLeftGridSlot->GetStackCount();
	}
	return CurrentSlotStackCount;
}

void UDkInventoryItemGrid::PutDownOnIndex(const int32 Index)
{
	AddItemToIndex(DraggedItem->GetInventoryItem(), Index, DraggedItem->GetStackCount(), DraggedItem->GetIsStackable());
	UpdateGridSlots(DraggedItem->GetInventoryItem(), Index, DraggedItem->GetStackCount(),
	                DraggedItem->GetIsStackable());
	ClearDraggedItem();
}

void UDkInventoryItemGrid::PopulateGrid(EInventoryItemCategory InCategory)
{
	if (!InventoryComponent.IsValid()) return;

	const TArray<FInventoryItemBriefInfo>* ItemBriefInfos = InventoryComponent->GetItemBriefInfoByCategory(InCategory);
	if (!ItemBriefInfos) return;

	for (const FInventoryItemBriefInfo& ItemBriefInfo : *ItemBriefInfos)
	{
		if (!ItemBriefInfo.IsValid() || ItemBriefInfo.IsEmpty()) continue;
		int Index = ItemBriefInfo.Index;
		if (!GridSlots.IsValidIndex(Index)) continue;
		UDkInventoryGridSlot* GridSlot = GridSlots[Index];

		UpdateGridSlotInfo(ItemBriefInfo, Index, GridSlot);
	}
}

void UDkInventoryItemGrid::HandleInventoryCategoryItemsArrayUpdated()
{
	if (!InventoryComponent.IsValid()) return;

	const TArray<FInventoryItemBriefInfo>* ItemBriefInfos =
		InventoryComponent->GetItemBriefInfoByCategory(ItemCategory);
	if (!ItemBriefInfos) return;

	for (UDkInventoryGridSlot* GridSlot : GridSlots)
	{
		FInventoryItemBriefInfo ItemBriefInfo = (*ItemBriefInfos)[GridSlot->GetTileIndex()];

		if (GridSlot->GetStackCount() != ItemBriefInfo.StackCount ||
			GridSlot->GetInventoryItem() != ItemBriefInfo.InventoryItem ||
			GridSlot->GetTileIndex() != ItemBriefInfo.Index)
		{
			UpdateGridSlotInfo(ItemBriefInfo, ItemBriefInfo.Index, GridSlot);
		}
	}
}

void UDkInventoryItemGrid::UpdateGridSlotInfo(const FInventoryItemBriefInfo& ItemBriefInfo, int Index, UDkInventoryGridSlot* GridSlot)
{
	const FInventoryItemFragment_Image* ImageFragment = GetFragment<FInventoryItemFragment_Image>(
		ItemBriefInfo.InventoryItem, DkGameplayTags::Dk_Inventory_Fragment_Icon
	);
	if (!ImageFragment) return;

	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = FVector2D(100.f);
	GridSlot->SetItemIcon(Brush);

	GridSlot->SetItemStackNum(ItemBriefInfo.StackCount);

	GridSlot->SetInventoryItem(ItemBriefInfo.InventoryItem);
	GridSlot->SetUpperLeftIndex(Index);
	GridSlot->SetOccupiedBrush();
	GridSlot->SetIsAvailable(false);
	GridSlot->SetStackCount(ItemBriefInfo.StackCount);
}

void UDkInventoryItemGrid::AddItem(UDkInventoryItem* Item)
{
	if (!IsValid(Item) || !MatchesCategory(Item)) return;

	FDkInventorySlotAvailabilityResult AvailabilityResult = HasRoomForItem(Item);
	AddItemToIndices(AvailabilityResult, Item);
}

void UDkInventoryItemGrid::HandleStackChanged(const FDkInventorySlotAvailabilityResult& Result)
{
	if (!Result.Item.IsValid() || !MatchesCategory(Result.Item.Get())) return;

	AddStacks(Result);
}

void UDkInventoryItemGrid::AddStacks(const FDkInventorySlotAvailabilityResult& Result)
{
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		if (Availability.bItemAtIndex)
		{
			const TObjectPtr<UDkInventoryGridSlot>& GridSlot = GridSlots[Availability.Index];
			GridSlot->SetItemStackNum(GridSlot->GetStackCount() + Availability.AmountToFill);
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
	const FInventoryItemFragment_Image* ImageFragment =
		GetFragment<FInventoryItemFragment_Image>(NewItem, DkGameplayTags::Dk_Inventory_Fragment_Icon);
	if (!ImageFragment) return;

	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = FVector2D(100.f);
	GridSlots[Index]->SetItemIcon(Brush);

	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	GridSlots[Index]->SetItemStackNum(StackUpdateAmount);
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
	GridSlot->SetInventoryItem(NewItem);
	GridSlot->SetUpperLeftIndex(Index);
	GridSlot->SetOccupiedBrush();
	GridSlot->SetIsAvailable(false);
	if (bStackable)
	{
		GridSlot->SetStackCount(StackAmount);
	}
}

void UDkInventoryItemGrid::ConstructGrid(int32 InRows, int32 InColumns)
{
	GridSlots.Reset();
	Rows = InRows;
	Columns = InColumns;
	GridSlots.Reserve(Rows * Columns);

	UniformGridPanel->ClearChildren();
	for (int32 j = 0; j < Rows; ++j)
	{
		for (int32 i = 0; i < Columns; ++i)
		{
			UDkInventoryGridSlot* GridSlot = CreateWidget<UDkInventoryGridSlot>(this, GridSlotClass);
			UniformGridPanel->AddChildToUniformGrid(GridSlot, j, i);

			GridSlot->SetGridSlotSize(TileSize);
			GridSlot->SetTileIndex(UDkInventoryFunctionLibrary::GetIndexFromPosition({i, j}, Columns));

			GridSlots.Add(GridSlot);
			// GridSlot->GridSlotHovered.AddDynamic(this, &ThisClass::OnGridSlotHovered);
			// GridSlot->GridSlotUnhovered.AddDynamic(this, &ThisClass::OnGridSlotUnhovered);
			GridSlot->GridSlotClicked.AddDynamic(this, &ThisClass::OnGridSlotClicked);
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
	if (!IsValid(DraggedItem)) return;

	UDkInventoryItem* TempInventoryItem = DraggedItem->GetInventoryItem();
	const int32 TempStackCount = DraggedItem->GetStackCount();
	const bool bTempIsStackable = DraggedItem->GetIsStackable();

	RequestSwapItem();

	AssignDraggedItem(ClickedInventoryItem, GridIndex, DraggedItem->GetPreviousGridIndex());
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
	AddItemToIndex(TempInventoryItem, ItemDropIndex, TempStackCount, bTempIsStackable);
	UpdateGridSlots(TempInventoryItem, ItemDropIndex, TempStackCount, bTempIsStackable);
}

void UDkInventoryItemGrid::ConsumeDraggedItemStack(int32 ClickedStackCount, int32 DraggedStackCount, int32 GridIndex)
{
	const int32 NewClickedStackCount = ClickedStackCount + DraggedStackCount;

	GridSlots[GridIndex]->SetStackCount(NewClickedStackCount);
	GridSlots[GridIndex]->SetItemStackNum(NewClickedStackCount);
	ClearDraggedItem();

	HighlightSlot(GridIndex);
}

void UDkInventoryItemGrid::DragItem(UDkInventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	if (!IsValid(ClickedInventoryItem)) return;

	AssignDraggedItem(ClickedInventoryItem, GridIndex, GridIndex);

	// 从背包中移除被点击的Item
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
}

void UDkInventoryItemGrid::AssignDraggedItem(UDkInventoryItem* InventoryItem)
{
	if (!IsValid(DraggedItem))
	{
		DraggedItem = CreateWidget<UDkInventoryDraggedItem>(GetOwningPlayer(), DraggedItemClass);
	}

	const FItemFragment_Grid* GridFragment =
		GetFragment<FItemFragment_Grid>(InventoryItem, DkGameplayTags::Dk_Inventory_Fragment_Grid);
	const FInventoryItemFragment_Image* ImageFragment =
		GetFragment<FInventoryItemFragment_Image>(InventoryItem, DkGameplayTags::Dk_Inventory_Fragment_Icon);
	if (!GridFragment || !ImageFragment) return;

	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = ImageFragment->GetIconDimension() - 2 * GridFragment->GetGridPadding();

	DraggedItem->SetImageBrush(Brush);
	DraggedItem->SetInventoryItem(InventoryItem);
	DraggedItem->SetIsStackable(InventoryItem->IsItemStackable());
	check(InventoryComponent.IsValid());
	InventoryComponent->OnDraggedItemCreated.Broadcast(DraggedItem);
	DraggedItem->OnDraggedItemClicked.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemClicked);

	DraggedItem->SetDesiredSizeInViewport(Brush.ImageSize);
	DraggedItem->AddToViewport();
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
	GridSlots[GridIndex]->SetInventoryItem(nullptr);
	GridSlots[GridIndex]->SetUpperLeftIndex(INDEX_NONE);
	GridSlots[GridIndex]->SetUnoccupiedBrush();
	GridSlots[GridIndex]->SetDefaultItemIcon();
	GridSlots[GridIndex]->SetIsAvailable(true);
	GridSlots[GridIndex]->SetStackCount(0);
	GridSlots[GridIndex]->SetItemStackNum(0);
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
	if (!IsValid(InDraggedItem)) return;
	if (!IsValid(InDraggedItem->GetInventoryItem()) || !MatchesCategory(InDraggedItem->GetInventoryItem())) return;

	if (!InDraggedItem->IsPreviousEquipped() && IsValid(InDraggedItem->GetInventoryItem()))
	{
		FDkInventorySlotAvailabilityResult AddItemResult = HasRoomForItem(InDraggedItem->GetInventoryItem());
		AddItemResult.Item = InDraggedItem->GetInventoryItem();
		if (AddItemResult.TotalRoomToFill == 0)
		{
			InventoryComponent->OnNoRoomInInventory.Broadcast(FText::FromString(TEXT("拖拽物品暂无空间放置，请整理后重新加入背包")));
			DropItem();
			return;
		}

		// 将Item添加到Inventory中
		if (AddItemResult.Item.IsValid() && AddItemResult.bStackable)
		{
			AddStacks(AddItemResult);
		}
		else
		{
			AddItem(InDraggedItem->GetInventoryItem());
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
	if (!IsValid(DraggedItem)) return;
	if (!GridSlots.IsValidIndex(ItemDropIndex)) return;
	if (DraggedItem->GetInventoryItem()->GetItemManifest().GetItemCategory() != ItemCategory) return;
	if (!bMouseWithInCanvas) return;

	if (IsValid(GridSlots[ItemDropIndex]->GetInventoryItem()))
	{
		if (DraggedItem->IsPreviousEquipped())
		{
			return;
		}
		OnGridSlotClicked(ItemDropIndex, MouseEvent);
		return;
	}

	UDkInventoryGridSlot* GridSlot = GridSlots[ItemDropIndex];
	if (!IsValid(GridSlot->GetInventoryItem()))
	{
		if (!DraggedItem->IsPreviousEquipped())
		{
			RequestMoveItem(DraggedItem->GetStackCount());
		}
		else
		{
			RequestMoveItemFromEquipment(DraggedItem->GetInventoryItem(), DraggedItem->GetStackCount());
		}
		PutDownOnIndex(ItemDropIndex);
	}
}

void UDkInventoryItemGrid::RequestMoveItem(int MoveStackCount)
{
	if (!IsValid(DraggedItem)) return;
	if (InventoryComponent.IsValid())
	{
		if (UDkPlayerInventoryComp* PlayerInventoryComp = Cast<UDkPlayerInventoryComp>(InventoryComponent.Get()))
		{
			PlayerInventoryComp->RequestMoveItem(
				ItemCategory, DraggedItem->GetPreviousGridIndex(), ItemDropIndex, MoveStackCount
			);
		}
	}
}

void UDkInventoryItemGrid::RequestMoveItemFromEquipment(UDkInventoryItem* Item, int MoveStackCount)
{
	if (!IsValid(DraggedItem)) return;
	if (InventoryComponent.IsValid())
	{
		if (UDkPlayerInventoryComp* PlayerInventoryComp = Cast<UDkPlayerInventoryComp>(InventoryComponent.Get()))
		{
			PlayerInventoryComp->RequestMoveItemFromEquipment(
				ItemCategory, Item, ItemDropIndex, MoveStackCount
			);
		}
	}
}

void UDkInventoryItemGrid::RequestSwapItem()
{
	if (!IsValid(DraggedItem)) return;
	if (InventoryComponent.IsValid())
	{
		if (UDkPlayerInventoryComp* PlayerInventoryComp = Cast<UDkPlayerInventoryComp>(InventoryComponent.Get()))
		{
			PlayerInventoryComp->RequestSwapItems(
				ItemCategory, DraggedItem->GetPreviousGridIndex(), ItemDropIndex, DraggedItem->GetStackCount()
			);
		}
	}
}

void UDkInventoryItemGrid::UpdateItemDropIndex(const FVector2D& PanelPosition, const FVector2D& MousePosition)
{
	// 如果鼠标不在UniformGridPanel内，return
	if (!bMouseWithInCanvas) return;

	if (!DraggedItem) return;
	if (DraggedItem->GetInventoryItem()->GetItemManifest().GetItemCategory() != ItemCategory) return;

	const FVector2D LocalMousePosition = MousePosition - PanelPosition;
	const float TotalTileWidth = TileSize + SlotDistance * 2;
	const float TotalTileHeight = TileSize + SlotDistance * 2;
	int32 ColumnIndex = FMath::FloorToInt(LocalMousePosition.X / TotalTileWidth);
	int32 RowIndex = FMath::FloorToInt(LocalMousePosition.Y / TotalTileHeight);
	// 鼠标在有效区域外
	if (ColumnIndex < 0 || ColumnIndex >= Columns || RowIndex < 0 || RowIndex >= Rows)
	{
		ItemDropIndex = INDEX_NONE;
		UnHighlightSlot(LastHighlightedIndex);
		return;
	}

	const float LocalX = FMath::Fmod(LocalMousePosition.X, TotalTileWidth);
	const float LocalY = FMath::Fmod(LocalMousePosition.Y, TotalTileHeight);
	// 如果鼠标在padding区域，可以选择不更新或者保持上一个有效索引
	if (LocalX < SlotDistance || LocalX > TileSize + SlotDistance ||
		LocalY < SlotDistance || LocalY > TileSize + SlotDistance)
	{
		ItemDropIndex = INDEX_NONE;
		UnHighlightSlot(LastHighlightedIndex);
		return;
	}

	ItemDropIndex = UDkInventoryFunctionLibrary::GetIndexFromPosition(
		FIntPoint(ColumnIndex, RowIndex),
		Columns
	);
	ChangeHoverType(ItemDropIndex, EInventoryGridSlotState::GrayedOut);
}

void UDkInventoryItemGrid::ChangeHoverType(const int32 InIndex, EInventoryGridSlotState GridSlotState)
{
	UnHighlightSlot(LastHighlightedIndex);
	switch (GridSlotState)
	{
	case EInventoryGridSlotState::Unoccupied:
		GridSlots[InIndex]->SetUnoccupiedBrush();
		break;
	case EInventoryGridSlotState::Occupied:
		GridSlots[InIndex]->SetUnoccupiedBrush();
		break;
	case EInventoryGridSlotState::Enabled:
		GridSlots[InIndex]->SetEnabledBrush();
		break;
	case EInventoryGridSlotState::Disabled:
		GridSlots[InIndex]->SetDisabledBrush();
		break;
	case EInventoryGridSlotState::GrayedOut:
		GridSlots[InIndex]->SetGrayedOutBrush();
		break;
	}

	LastHighlightedIndex = InIndex;
}

bool UDkInventoryItemGrid::CursorExitedCanvas(
	const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& MousePos)
{
	bLastMouseWithInCanvas = bMouseWithInCanvas;
	bMouseWithInCanvas = UDkUIFunctionLibrary::IsWithInBounds(BoundaryPos, BoundarySize, MousePos);
	if (!bMouseWithInCanvas && bLastMouseWithInCanvas)
	{
		UnHighlightSlot(LastHighlightedIndex);
		return true;
	}
	return false;
}

void UDkInventoryItemGrid::HighlightSlot(const int32 Index)
{
	if (!bMouseWithInCanvas) return;
	if (!GridSlots.IsValidIndex(Index)) return;
	UnHighlightSlot(LastHighlightedIndex);
	GridSlots[Index]->SetOccupiedBrush();

	LastHighlightedIndex = Index;
}

void UDkInventoryItemGrid::UnHighlightSlot(const int32 Index)
{
	if (!GridSlots.IsValidIndex(Index)) return;
	if (GridSlots[Index]->IsAvailable())
	{
		GridSlots[Index]->SetUnoccupiedBrush();
	}
	else
	{
		GridSlots[Index]->SetOccupiedBrush();
	}
}

void UDkInventoryItemGrid::OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(DraggedItem)) return;

	UDkInventoryGridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->SetOccupiedBrush();
	}
}

void UDkInventoryItemGrid::OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(DraggedItem)) return;

	UDkInventoryGridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->SetUnoccupiedBrush();
	}
}

void UDkInventoryItemGrid::OnGridSlotClicked(int GridIndex, const FPointerEvent& MouseEvent)
{
	check(GridSlots.IsValidIndex(GridIndex));
	UDkInventoryItem* ClickedInventoryItem = GridSlots[GridIndex]->GetInventoryItem();
	if (!IsValid(DraggedItem) && UDkCommonFunctionLibrary::IsLeftMouseClick(MouseEvent))
	{
		// 拖拽Item
		DragItem(ClickedInventoryItem, GridIndex);
		return;
	}

	if (UDkCommonFunctionLibrary::IsRightMouseClick(MouseEvent))
	{
		if (!IsValid(DraggedItem))
		{
			CreateItemPopUp(GridIndex);
		}
		return;
	}

	// DraggedItem和被点击的Item是一个类型吗，他们都可堆叠吗？
	if (IsSameStackableWithDraggedItem(ClickedInventoryItem))
	{
		const int32 ClickedStackCount = GridSlots[GridIndex]->GetStackCount();
		const FItemFragment_Stackable* StackableFragment =
			ClickedInventoryItem->GetItemManifest().GetFragmentOfType<FItemFragment_Stackable>();
		const int32 MaxStackSize = StackableFragment->GetMaxStackSize();
		const int32 SpaceInClickedSlot = MaxStackSize - ClickedStackCount;
		const int32 DraggedStackCount = DraggedItem->GetStackCount();
		// 是否应该交换SlottedItem和DraggedItem
		if (SpaceInClickedSlot == 0 && DraggedStackCount < MaxStackSize)
		{
			// 交换SlottedItem和DraggedItem
			UDkInventoryGridSlot* GridSlot = GridSlots[GridIndex];
			GridSlot->SetStackCount(DraggedStackCount);
			GridSlot->SetItemStackNum(DraggedStackCount);

			DraggedItem->UpdateStackCount(ClickedStackCount);

			RequestSwapItem();

			return;
		}

		//	是否可以合并DraggedItem
		if (SpaceInClickedSlot >= DraggedStackCount)
		{
			RequestMoveItem(DraggedItem->GetStackCount());
			ConsumeDraggedItemStack(ClickedStackCount, DraggedStackCount, GridIndex);
			return;
		}

		//	是否可以填充ClickedItem,并更新DraggedItem
		if (SpaceInClickedSlot < DraggedStackCount)
		{
			// 填充ClickedItem,并更新DraggedItem
			UDkInventoryGridSlot* GridSlot = GridSlots[GridIndex];
			GridSlot->SetStackCount(MaxStackSize);
			GridSlot->SetItemStackNum(MaxStackSize);

			DraggedItem->UpdateStackCount(DraggedStackCount - SpaceInClickedSlot);

			RequestMoveItem(SpaceInClickedSlot);

			return;
		}

		return;
	}
	// 和DraggedItem交换位置
	SwapWithDraggedItem(ClickedInventoryItem, GridIndex);
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
	UDkInventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem();
	if (!IsValid(RightClickedItem)) return;
	if (!RightClickedItem->IsItemStackable()) return;

	const int32 UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	UDkInventoryGridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
	const int32 StackCount = UpperLeftGridSlot->GetStackCount();
	const int32 NewStackCount = StackCount - SplitAmount;

	UpperLeftGridSlot->SetStackCount(NewStackCount);
	UpperLeftGridSlot->SetItemStackNum(NewStackCount);

	AssignDraggedItem(RightClickedItem, UpperLeftIndex, UpperLeftIndex);
	DraggedItem->UpdateStackCount(SplitAmount);
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

	InventoryComponent->ServerDropItem(DraggedItem);

	ClearDraggedItem();
}

void UDkInventoryItemGrid::HandlePopUpMenuConsume(int32 Index)
{
	OnPopUpMenuConsume(Index);
}

void UDkInventoryItemGrid::OnPopUpMenuConsume(int32 Index)
{
	UDkInventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem();
	if (!IsValid(RightClickedItem)) return;

	const int32 UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	UDkInventoryGridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
	const int32 NewStackCount = UpperLeftGridSlot->GetStackCount() - 1;

	UpperLeftGridSlot->SetStackCount(NewStackCount);
	UpperLeftGridSlot->SetItemStackNum(NewStackCount);

	InventoryComponent->ServerConsumeItem(RightClickedItem, Index);

	if (NewStackCount <= 0)
	{
		RemoveItemFromGrid(RightClickedItem, Index);
	}
}
