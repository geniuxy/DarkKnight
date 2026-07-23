// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DKInventoryItemSpatialGrid.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/InventoryComps/DkInventoryComponent.h"
#include "FunctionLibrarys/DkCommonFunctionLibrary.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Widgets/Inventory/DkInventoryDraggedItem.h"
#include "Widgets/Inventory/DkInventoryGridSlot.h"
#include "Widgets/Inventory/DkInventorySlottedItem.h"

void UDKInventoryItemSpatialGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UCommonUserWidget::NativeTick(MyGeometry, InDeltaTime);

	// 根据鼠标的位置，更改Hover的格子样式
	const FVector2D CanvasPosition = UDkUIFunctionLibrary::GetWidgetPosition(GridCanvasPanel);
	// 这里的MousePosition是逻辑/虚拟像素大小，不受DPI影响
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	if (CursorExitedCanvas(CanvasPosition, UDkUIFunctionLibrary::GetWidgetSize(GridCanvasPanel), MousePosition))
	{
		return;
	}

	UpdateTileParameters(CanvasPosition, MousePosition);
}

void UDKInventoryItemSpatialGrid::AddStacks(const FDkInventorySlotAvailabilityResult& Result)
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

void UDKInventoryItemSpatialGrid::AddItemToIndex(
	UDkInventoryItem* NewItem, int32 Index, int32 StackAmount, bool bStackable)
{
	const FItemFragment_Grid* GridFragment =
		GetFragment<FItemFragment_Grid>(NewItem, DkGameplayTags::Dk_Inventory_Fragment_Grid);
	const FInventoryItemFragment_Image* ImageFragment =
		GetFragment<FInventoryItemFragment_Image>(NewItem, DkGameplayTags::Dk_Inventory_Fragment_Icon);
	if (!GridFragment || !ImageFragment) return;

	// 创建slotted item
	UDkInventorySlottedItem* SlottedItem = CreateWidget<UDkInventorySlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetInventoryItem(NewItem);
	SlottedItem->SetGridIndex(Index);
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
	SlottedItem->SetbIsStackable(bStackable);
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItem->UpdateStackCount(StackUpdateAmount);
	SlottedItem->OnSlottedItemClicked.AddDynamic(this, &ThisClass::HandleSlottedItemClicked);

	// 把slotted item加到CanvasPanel
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);

	// 把slotted item存入Map中
	SlottedItemMap.Add(Index, SlottedItem);
}

FVector2D UDKInventoryItemSpatialGrid::GetDrawSize(const FItemFragment_Grid* GridFragment) const
{
	return GridFragment->GetGridSize() * TileSize - GridFragment->GetGridPadding() * 2;
}

void UDKInventoryItemSpatialGrid::AddSlottedItemToCanvas(
	const int32 Index, const FItemFragment_Grid* GridFragment, UDkInventorySlottedItem* SlottedItem) const
{
	GridCanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));
	const FVector2D DrawPos = UDkInventoryFunctionLibrary::GetPositionFormIndex(Index, Columns) * TileSize;
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D(GridFragment->GetGridPadding());
	CanvasSlot->SetPosition(DrawPosWithPadding);
}

bool UDKInventoryItemSpatialGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimension) const
{
	if (StartIndex < 0 || StartIndex >= GridSlots.Num()) return false;
	const int32 EndColumn = (StartIndex % Columns) + ItemDimension.X - 1;
	const int32 EndRow = (StartIndex / Columns) + ItemDimension.Y - 1;
	return EndColumn <= Columns - 1 && EndRow <= Rows - 1;
}

bool UDKInventoryItemSpatialGrid::CheckSlotConstraints(
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

bool UDKInventoryItemSpatialGrid::HasRoomAtIndex(
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

FDkInventorySlotAvailabilityResult UDKInventoryItemSpatialGrid::HasRoomForItem(const FInventoryItemManifest& Manifest)
{
	FDkInventorySlotAvailabilityResult Result;
	// 判断物品是否可堆叠
	const FItemFragment_Stackable* StackableFragment =
		Manifest.GetFragmentOfType<FItemFragment_Stackable>();
	Result.bStackable = StackableFragment != nullptr;
	// 确定需要添加多少StackCount。AmountToFill
	const int32 MaxStackCount = StackableFragment ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = StackableFragment ? StackableFragment->GetStackCount() : 1;

	TSet<int32> CheckedIndices;
	// For each Grid Slot:
	for (const auto& GridSlot : GridSlots)
	{
		//   如果已经没有剩余要填充的数量，提前跳出循环。
		if (AmountToFill == 0) break;
		//   该索引是否已被占用(claimed)？
		if (CheckedIndices.Contains(GridSlot->GetTileIndex())) continue;

		const FItemFragment_Grid* GridFragment = Manifest.GetFragmentOfType<FItemFragment_Grid>();
		const FIntPoint& Dimension = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
		//   是否超出网格边界？
		if (!IsInGridBounds(GridSlot->GetTileIndex(), Dimension)) continue;
		//   物品能否放进这里？
		TSet<int32> TemporarilyClaimed;
		if (!HasRoomAtIndex(GridSlot, Dimension, CheckedIndices, TemporarilyClaimed,
		                    Manifest.GetItemTag(), MaxStackCount))
		{
			continue;
		}
		//   需要填充多少？如果是0，说明这个格子不需要考虑
		const int32 AmountToFillInSlot =
			CalculateFillAmountForSlot(Result.bStackable, MaxStackCount, AmountToFill, GridSlot);
		if (AmountToFillInSlot == 0) continue;
		//   更新已检查索引数组
		CheckedIndices.Append(TemporarilyClaimed);
		//   更新SlotAvailabilityResult
		Result.TotalRoomToFill += AmountToFillInSlot;
		Result.SlotAvailabilities.Emplace(
			IsValid(GridSlot->GetInventoryItem()) ? GridSlot->GetUpperLeftIndex() : GridSlot->GetTileIndex(),
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

void UDKInventoryItemSpatialGrid::PutDownOnIndex(const int32 Index)
{
	Super::PutDownOnIndex(Index);
}

void UDKInventoryItemSpatialGrid::UpdateGridSlots(
	UDkInventoryItem* NewItem, const int32 Index, int32 StackAmount, bool bStackable)
{
	check(GridSlots.IsValidIndex(Index));

	if (bStackable)
	{
		GridSlots[Index]->SetStackCount(StackAmount);
	}

	const FItemFragment_Grid* GridFragment =
		GetFragment<FItemFragment_Grid>(NewItem, DkGameplayTags::Dk_Inventory_Fragment_Grid);
	if (!GridFragment) return;

	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

	UDkInventoryFunctionLibrary::ForEach2D(
		GridSlots, Index, Dimensions, Columns,
		[&](UDkInventoryGridSlot* GridSlot) // [&]表示以引用的方式捕获当前作用域中的所有局部变量
		{
			GridSlot->SetInventoryItem(NewItem);
			GridSlot->SetUpperLeftIndex(Index);
			GridSlot->SetOccupiedBrush();
			GridSlot->SetIsAvailable(false);
		}
	);
}


void UDKInventoryItemSpatialGrid::ConstructGrid(int32 InRows, int32 InColumns)
{
	GridSlots.Reset();
	Rows = InRows;
	Columns = InColumns;
	GridSlots.Reserve(Rows * Columns);

	for (int32 j = 0; j < Rows; ++j)
	{
		for (int32 i = 0; i < Columns; ++i)
		{
			UDkInventoryGridSlot* GridSlot = CreateWidget<UDkInventoryGridSlot>(this, GridSlotClass);
			GridCanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition(i, j);
			GridSlot->SetTileIndex(UDkInventoryFunctionLibrary::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize(FVector2D(TileSize));
			GridCPS->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
			GridSlot->GridSlotHovered.AddDynamic(this, &ThisClass::OnGridSlotHovered);
			GridSlot->GridSlotUnhovered.AddDynamic(this, &ThisClass::OnGridSlotUnhovered);
		}
	}
}

void UDKInventoryItemSpatialGrid::RemoveItemFromGrid(UDkInventoryItem* InventoryItem, const int32 GridIndex)
{
	const FItemFragment_Grid* GridFragment =
		GetFragment<FItemFragment_Grid>(InventoryItem, DkGameplayTags::Dk_Inventory_Fragment_Grid);
	if (!GridFragment) return;

	UDkInventoryFunctionLibrary::ForEach2D(
		GridSlots, GridIndex, GridFragment->GetGridSize(), Columns,
		[&](UDkInventoryGridSlot* GridSlot)
		{
			GridSlot->SetInventoryItem(nullptr);
			GridSlot->SetUpperLeftIndex(INDEX_NONE);
			GridSlot->SetUnoccupiedBrush();
			GridSlot->SetIsAvailable(true);
			GridSlot->SetStackCount(0);
		}
	);

	if (SlottedItemMap.Contains(GridIndex))
	{
		TObjectPtr<UDkInventorySlottedItem> FoundSlottedItem;
		SlottedItemMap.RemoveAndCopyValue(GridIndex, FoundSlottedItem);
		FoundSlottedItem->RemoveFromParent();
	}
}

void UDKInventoryItemSpatialGrid::HandleSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	OnSlottedItemClicked(GridIndex, MouseEvent);
}

void UDKInventoryItemSpatialGrid::OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
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

			UDkInventorySlottedItem* SlottedItem = SlottedItemMap.FindChecked(GridIndex);
			SlottedItem->UpdateStackCount(DraggedStackCount);

			DraggedItem->UpdateStackCount(ClickedStackCount);

			return;
		}

		//	是否可以合并DraggedItem
		if (SpaceInClickedSlot >= DraggedStackCount)
		{
			ConsumeDraggedItemStack(ClickedStackCount, DraggedStackCount, GridIndex);
			return;
		}

		//	是否可以填充ClickedItem,并更新DraggedItem
		if (SpaceInClickedSlot < DraggedStackCount)
		{
			// 填充ClickedItem,并更新DraggedItem
			UDkInventoryGridSlot* GridSlot = GridSlots[GridIndex];
			GridSlot->SetStackCount(MaxStackSize);

			UDkInventorySlottedItem* SlottedItem = SlottedItemMap.FindChecked(GridIndex);
			SlottedItem->UpdateStackCount(MaxStackSize);

			DraggedItem->UpdateStackCount(DraggedStackCount - SpaceInClickedSlot);

			return;
		}

		return;
	}
	// 和DraggedItem交换位置
	SwapWithDraggedItem(ClickedInventoryItem, GridIndex);
}

void UDKInventoryItemSpatialGrid::SwapWithDraggedItem(UDkInventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	Super::SwapWithDraggedItem(ClickedInventoryItem, GridIndex);
}

void UDKInventoryItemSpatialGrid::ConsumeDraggedItemStack(
	int32 ClickedStackCount, int32 DraggedStackCount, int32 GridIndex)
{
	const int32 NewClickedStackCount = ClickedStackCount + DraggedStackCount;

	GridSlots[GridIndex]->SetStackCount(NewClickedStackCount);
	SlottedItemMap.FindChecked(GridIndex)->UpdateStackCount(NewClickedStackCount);
	ClearDraggedItem();

	const FItemFragment_Grid* GridFragment =
		GridSlots[GridIndex]->GetInventoryItem()->GetItemManifest().GetFragmentOfType<FItemFragment_Grid>();
	const FIntPoint Dimension = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

	HighlightSlots(GridIndex, Dimension);
}

void UDKInventoryItemSpatialGrid::AssignDraggedItem(UDkInventoryItem* InventoryItem)
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
	Brush.ImageSize = GetDrawSize(GridFragment);

	DraggedItem->SetImageBrush(Brush);
	DraggedItem->SetGridDimension(GridFragment->GetGridSize());
	DraggedItem->SetInventoryItem(InventoryItem);
	DraggedItem->SetIsStackable(InventoryItem->IsItemStackable());
	check(InventoryComponent.IsValid());
	InventoryComponent->OnDraggedItemCreated.Broadcast(DraggedItem);
	DraggedItem->OnDraggedItemClicked.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemClicked);

	DraggedItem->SetDesiredSizeInViewport(Brush.ImageSize);
	DraggedItem->AddToViewport();
}

void UDKInventoryItemSpatialGrid::AssignDraggedItem(
	UDkInventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex)
{
	Super::AssignDraggedItem(InventoryItem, GridIndex, PreviousGridIndex);
}

void UDKInventoryItemSpatialGrid::OnDraggedItemClicked(const FPointerEvent& MouseEvent)
{
	if (!IsValid(DraggedItem)) return;
	if (!GridSlots.IsValidIndex(ItemDropIndex)) return;
	if (!IsInGridBounds(ItemDropIndex, DraggedItem->GetGridDimension())) return;
	if (DraggedItem->GetInventoryItem()->GetItemManifest().GetItemCategory() != ItemCategory) return;
	if (!bMouseWithInCanvas) return;

	if (CurrentSpaceQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentSpaceQueryResult.UpperLeftIndex))
	{
		if (DraggedItem->IsPreviousEquipped())
		{
			return;
		}
		OnSlottedItemClicked(CurrentSpaceQueryResult.UpperLeftIndex, MouseEvent);
		return;
	}

	UDkInventoryGridSlot* GridSlot = GridSlots[ItemDropIndex];
	if (!IsValid(GridSlot->GetInventoryItem()) && CurrentSpaceQueryResult.bHasSpace)
	{
		PutDownOnIndex(ItemDropIndex);
	}
}

void UDKInventoryItemSpatialGrid::UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition)
{
	// 如果鼠标不在GridCanvasPanel内，return
	if (!bMouseWithInCanvas) return;

	// 计算网格的象限、索引、坐标
	const FIntPoint HoveredTileCoordinate = CalculateHoveredCoordinates(CanvasPosition, MousePosition);

	LastTileParameters = TileParameters;
	TileParameters.TileCoordinate = HoveredTileCoordinate;
	TileParameters.TileIndex = UDkInventoryFunctionLibrary::GetIndexFromPosition(HoveredTileCoordinate, Columns);
	TileParameters.TileQuadrant = CalculateTileQuadrant(CanvasPosition, MousePosition);

	OnTileParametersUpdated(TileParameters);
}

FIntPoint UDKInventoryItemSpatialGrid::CalculateHoveredCoordinates(
	const FVector2D& CanvasPosition, const FVector2D& MousePosition) const
{
	return FIntPoint{
		static_cast<int32>(FMath::FloorToInt((MousePosition.X - CanvasPosition.X) / TileSize)),
		static_cast<int32>(FMath::FloorToInt((MousePosition.Y - CanvasPosition.Y) / TileSize))
	};
}

EInventoryTileQuadrant UDKInventoryItemSpatialGrid::CalculateTileQuadrant(
	const FVector2D& CanvasPosition, const FVector2D& MousePosition) const
{
	// 计算在当前格子的相对坐标
	const float TileLocalX = FMath::Fmod(MousePosition.X - CanvasPosition.X, TileSize);
	const float TileLocalY = FMath::Fmod(MousePosition.Y - CanvasPosition.Y, TileSize);

	// 决定了鼠标在当前格子的哪个象限
	const bool bIsTop = TileLocalY < TileSize / 2.f;
	const bool bIsLeft = TileLocalX < TileSize / 2.f;

	EInventoryTileQuadrant HoveredTileQuadrant = EInventoryTileQuadrant::None;
	if (bIsTop && bIsLeft) HoveredTileQuadrant = EInventoryTileQuadrant::TopLeft;
	else if (bIsTop && !bIsLeft) HoveredTileQuadrant = EInventoryTileQuadrant::TopRight;
	else if (!bIsTop && bIsLeft) HoveredTileQuadrant = EInventoryTileQuadrant::BottomLeft;
	else if (!bIsTop && !bIsLeft) HoveredTileQuadrant = EInventoryTileQuadrant::BottomRight;

	return HoveredTileQuadrant;
}

void UDKInventoryItemSpatialGrid::OnTileParametersUpdated(const FInventoryTileParameters& Parameters)
{
	if (!DraggedItem) return;
	if (DraggedItem->GetInventoryItem()->GetItemManifest().GetItemCategory() != ItemCategory) return;

	// 获取DraggedItem的尺寸/面积
	const FIntPoint Dimension = DraggedItem->GetGridDimension();
	// 计算DraggedItem所在背景位置的起始Index
	const FIntPoint StartingCoordinate =
		CalculateStartingCoordinate(Parameters.TileCoordinate, Dimension, Parameters.TileQuadrant);
	ItemDropIndex = UDkInventoryFunctionLibrary::GetIndexFromPosition(StartingCoordinate, Columns);

	CurrentSpaceQueryResult = CheckHoverPosition(Dimension);
	if (CurrentSpaceQueryResult.bHasSpace)
	{
		HighlightSlots(ItemDropIndex, Dimension);
		return;
	}
	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimension);

	if (CurrentSpaceQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentSpaceQueryResult.UpperLeftIndex))
	{
		const FItemFragment_Grid* GridFragment = GetFragment<FItemFragment_Grid>(
			CurrentSpaceQueryResult.ValidItem.Get(), DkGameplayTags::Dk_Inventory_Fragment_Grid
		);
		if (!GridFragment) return;

		ChangeHoverType(
			CurrentSpaceQueryResult.UpperLeftIndex,
			GridFragment->GetGridSize(),
			EInventoryGridSlotState::GrayedOut
		);
	}
}

FIntPoint UDKInventoryItemSpatialGrid::CalculateStartingCoordinate(
	const FIntPoint& Coordinate, const FIntPoint& Dimension, const EInventoryTileQuadrant Quadrant) const
{
	const int32 HasEvenWidth = Dimension.X % 2 == 0 ? 1 : 0;
	const int32 HasEvenHeight = Dimension.Y % 2 == 0 ? 1 : 0;

	FIntPoint StartingCoord{};
	switch (Quadrant)
	{
	case EInventoryTileQuadrant::TopLeft:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimension.X);
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimension.Y);
		break;
	case EInventoryTileQuadrant::TopRight:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimension.X) + HasEvenWidth;
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimension.Y);
		break;
	case EInventoryTileQuadrant::BottomLeft:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimension.X);
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimension.Y) + HasEvenHeight;
		break;
	case EInventoryTileQuadrant::BottomRight:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimension.X) + HasEvenWidth;
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimension.Y) + HasEvenHeight;
		break;
	case EInventoryTileQuadrant::None:
		Debug::Print(TEXT("无效的象限"));
		return FIntPoint(-1, -1);
	}

	return StartingCoord;
}

FInventorySpaceQueryResult UDKInventoryItemSpatialGrid::CheckHoverPosition(const FIntPoint& Dimension)
{
	FInventorySpaceQueryResult Result;
	// 是否在背包网格边界内
	if (ItemDropIndex == INDEX_NONE || !IsInGridBounds(ItemDropIndex, Dimension))
	{
		return Result;
	}

	Result.bHasSpace = true;
	// 查看DraggedItem背后的是否有别的Item，如果有的话将左上角坐标存进Set中
	TSet<int32> OccupiedUpperLeftIndices;
	UDkInventoryFunctionLibrary::ForEach2D(
		GridSlots, ItemDropIndex, Dimension, Columns,
		[&](const UDkInventoryGridSlot* CurrentSlot)
		{
			if (IsValid(CurrentSlot->GetInventoryItem()))
			{
				OccupiedUpperLeftIndices.Add(CurrentSlot->GetUpperLeftIndex());
				Result.bHasSpace = false;
			}
		}
	);

	// 如果有的话，是否只有一个Item? (即是否可能可以交换)
	if (OccupiedUpperLeftIndices.Num() == 1)
	{
		const int32 Index = *OccupiedUpperLeftIndices.CreateIterator();
		Result.ValidItem = GridSlots[Index]->GetInventoryItem();
		Result.UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	}

	return Result;
}

void UDKInventoryItemSpatialGrid::ChangeHoverType(
	const int32 StartingIndex, const FIntPoint& Dimension, EInventoryGridSlotState GridSlotState)
{
	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimension);
	UDkInventoryFunctionLibrary::ForEach2D(
		GridSlots, StartingIndex, Dimension, Columns,
		[GridSlotState](UDkInventoryGridSlot* CurrentSlot)
		{
			switch (GridSlotState)
			{
			case EInventoryGridSlotState::Unoccupied:
				CurrentSlot->SetUnoccupiedBrush();
				break;
			case EInventoryGridSlotState::Occupied:
				CurrentSlot->SetUnoccupiedBrush();
				break;
			case EInventoryGridSlotState::Enabled:
				CurrentSlot->SetEnabledBrush();
				break;
			case EInventoryGridSlotState::Disabled:
				CurrentSlot->SetDisabledBrush();
				break;
			case EInventoryGridSlotState::GrayedOut:
				CurrentSlot->SetGrayedOutBrush();
				break;
			}
		}
	);

	LastHighlightedIndex = StartingIndex;
	LastHighlightedDimension = Dimension;
}

bool UDKInventoryItemSpatialGrid::CursorExitedCanvas(
	const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& MousePos)
{
	bLastMouseWithInCanvas = bMouseWithInCanvas;
	bMouseWithInCanvas = UDkUIFunctionLibrary::IsWithInBounds(BoundaryPos, BoundarySize, MousePos);
	if (!bMouseWithInCanvas && bLastMouseWithInCanvas)
	{
		UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimension);
		return true;
	}
	return false;
}

void UDKInventoryItemSpatialGrid::HighlightSlots(const int32 Index, const FIntPoint& Dimension)
{
	if (!bMouseWithInCanvas) return;
	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimension);
	UDkInventoryFunctionLibrary::ForEach2D(
		GridSlots, Index, Dimension, Columns,
		[](UDkInventoryGridSlot* CurrentSlot)
		{
			CurrentSlot->SetOccupiedBrush();
		}
	);
	LastHighlightedIndex = Index;
	LastHighlightedDimension = Dimension;
}

void UDKInventoryItemSpatialGrid::UnHighlightSlots(const int32 Index, const FIntPoint& Dimension)
{
	UDkInventoryFunctionLibrary::ForEach2D(
		GridSlots, Index, Dimension, Columns,
		[](UDkInventoryGridSlot* CurrentSlot)
		{
			if (CurrentSlot->IsAvailable())
			{
				CurrentSlot->SetUnoccupiedBrush();
			}
			else
			{
				CurrentSlot->SetOccupiedBrush();
			}
		}
	);
}

void UDKInventoryItemSpatialGrid::OnPopUpMenuSplit(int32 SplitAmount, int32 Index)
{
	UDkInventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem();
	if (!IsValid(RightClickedItem)) return;
	if (!RightClickedItem->IsItemStackable()) return;

	const int32 UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	UDkInventoryGridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
	const int32 StackCount = UpperLeftGridSlot->GetStackCount();
	const int32 NewStackCount = StackCount - SplitAmount;

	UpperLeftGridSlot->SetStackCount(NewStackCount);
	SlottedItemMap.FindChecked(UpperLeftIndex)->UpdateStackCount(NewStackCount);

	AssignDraggedItem(RightClickedItem, UpperLeftIndex, UpperLeftIndex);
	DraggedItem->UpdateStackCount(SplitAmount);
}

void UDKInventoryItemSpatialGrid::OnPopUpMenuConsume(int32 Index)
{
	UDkInventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem();
	if (!IsValid(RightClickedItem)) return;

	const int32 UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	UDkInventoryGridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
	const int32 NewStackCount = UpperLeftGridSlot->GetStackCount() - 1;

	UpperLeftGridSlot->SetStackCount(NewStackCount);
	SlottedItemMap.FindChecked(UpperLeftIndex)->UpdateStackCount(NewStackCount);

	InventoryComponent->ServerConsumeItem(RightClickedItem);

	if (NewStackCount <= 0)
	{
		RemoveItemFromGrid(RightClickedItem, Index);
	}
}
