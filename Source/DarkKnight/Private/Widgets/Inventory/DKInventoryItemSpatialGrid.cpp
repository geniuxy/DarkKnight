// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DKInventoryItemSpatialGrid.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Widgets/Inventory/DkInventoryDraggedItem.h"
#include "Widgets/Inventory/DkInventoryGridSlot.h"
#include "Widgets/Inventory/DkInventorySlottedItem.h"

void UDKInventoryItemSpatialGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 根据鼠标的位置，更改Hover的格子样式
	const FVector2D CanvasPosition = UDkInventoryFunctionLibrary::GetWidgetPosition(GridCanvasPanel);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	if (CursorExitedCanvas(CanvasPosition, UDkInventoryFunctionLibrary::GetWidgetSize(GridCanvasPanel), MousePosition))
	{
		return;
	}

	UpdateTileParameters(CanvasPosition, MousePosition);
}

void UDKInventoryItemSpatialGrid::AddItemToIndex(
	UDkInventoryItem* NewItem, int32 Index, int32 StackAmount, bool bStackable)
{
	const FInventoryItemGridFragment* GridFragment =
		GetFragment<FInventoryItemGridFragment>(NewItem, DkGameplayTags::Dk_Inventory_Fragment_Grid);
	const FInventoryItemImageFragment* ImageFragment =
		GetFragment<FInventoryItemImageFragment>(NewItem, DkGameplayTags::Dk_Inventory_Fragment_Icon);
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

FVector2D UDKInventoryItemSpatialGrid::GetDrawSize(const FInventoryItemGridFragment* GridFragment) const
{
	return GridFragment->GetGridSize() * TileSize - GridFragment->GetGridPadding() * 2;
}

void UDKInventoryItemSpatialGrid::AddSlottedItemToCanvas(
	const int32 Index, const FInventoryItemGridFragment* GridFragment, UDkInventorySlottedItem* SlottedItem) const
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

int32 UDKInventoryItemSpatialGrid::CalculateFillAmountForSlot(
	const bool bStackable, const int32 MaxStackSize,
	const int32 AmountToFill, const UDkInventoryGridSlot* GridSlot) const
{
	const int32 RoomInSlot = MaxStackSize - GetStackAmount(GridSlot);
	return bStackable ? FMath::Min(AmountToFill, RoomInSlot) : 1;
}

int32 UDKInventoryItemSpatialGrid::GetStackAmount(const UDkInventoryGridSlot* GridSlot) const
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

FDkInventorySlotAvailabilityResult UDKInventoryItemSpatialGrid::HasRoomForItem(const FInventoryItemManifest& Manifest)
{
	FDkInventorySlotAvailabilityResult Result;
	// 判断物品是否可堆叠
	const FInventoryItemStackableFragment* StackableFragment =
		Manifest.GetFragmentOfType<FInventoryItemStackableFragment>();
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

		const FInventoryItemGridFragment* GridFragment = Manifest.GetFragmentOfType<FInventoryItemGridFragment>();
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

void UDKInventoryItemSpatialGrid::UpdateGridSlots(
	UDkInventoryItem* NewItem, const int32 Index, int32 StackAmount, bool bStackable)
{
	check(GridSlots.IsValidIndex(Index));

	if (bStackable)
	{
		GridSlots[Index]->SetStackCount(StackAmount);
	}

	const FInventoryItemGridFragment* GridFragment =
		GetFragment<FInventoryItemGridFragment>(NewItem, DkGameplayTags::Dk_Inventory_Fragment_Grid);
	if (!GridFragment) return;

	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

	UDkInventoryFunctionLibrary::ForEach2D(
		GridSlots, Index, Dimensions, Columns,
		[&](UDkInventoryGridSlot* GridSlot) // [&]表示以引用的方式捕获当前作用域中的所有局部变量
		{
			GridSlot->SetInventoryItem(NewItem);
			GridSlot->SetUpperLeftIndex(Index);
			GridSlot->SetOccupiedTexture();
			GridSlot->SetIsAvailable(false);
		}
	);
}


void UDKInventoryItemSpatialGrid::ConstructGrid()
{
	GridSlots.Reset();
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
	const FInventoryItemGridFragment* GridFragment =
		GetFragment<FInventoryItemGridFragment>(InventoryItem, DkGameplayTags::Dk_Inventory_Fragment_Grid);
	if (!GridFragment) return;

	UDkInventoryFunctionLibrary::ForEach2D(
		GridSlots, GridIndex, GridFragment->GetGridSize(), Columns,
		[&](UDkInventoryGridSlot* GridSlot)
		{
			GridSlot->SetInventoryItem(nullptr);
			GridSlot->SetUpperLeftIndex(INDEX_NONE);
			GridSlot->SetUnoccupiedTexture();
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

void UDKInventoryItemSpatialGrid::SwapWithDraggedItem(UDkInventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	if (!IsValid(DraggedItem)) return;

	UDkInventoryItem* TempInventoryItem = DraggedItem->GetInventoryItem();
	const int32 TempStackCount = DraggedItem->GetStackCount();
	const bool bTempIsStackable = DraggedItem->GetIsStackable();

	AssignDraggedItem(ClickedInventoryItem, GridIndex, DraggedItem->GetPreviousGridIndex());
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
	AddItemToIndex(TempInventoryItem, ItemDropIndex, TempStackCount, bTempIsStackable);
	UpdateGridSlots(TempInventoryItem, ItemDropIndex, TempStackCount, bTempIsStackable);
}

void UDKInventoryItemSpatialGrid::ConsumeDraggedItemStack(
	int32 ClickedStackCount, int32 DraggedStackCount, int32 GridIndex)
{
	const int32 NewClickedStackCount = ClickedStackCount + DraggedStackCount;

	GridSlots[GridIndex]->SetStackCount(NewClickedStackCount);
	SlottedItemMap.FindChecked(GridIndex)->UpdateStackCount(NewClickedStackCount);
	ClearDraggedItem();

	const FInventoryItemGridFragment* GridFragment =
		GridSlots[GridIndex]->GetInventoryItem()->GetItemManifest().GetFragmentOfType<FInventoryItemGridFragment>();
	const FIntPoint Dimension = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

	HighlightSlots(GridIndex, Dimension);
}

void UDKInventoryItemSpatialGrid::AssignDraggedItem(UDkInventoryItem* InventoryItem)
{
	if (!IsValid(DraggedItem))
	{
		DraggedItem = CreateWidget<UDkInventoryDraggedItem>(GetOwningPlayer(), DraggedItemClass);
	}

	const FInventoryItemGridFragment* GridFragment =
		GetFragment<FInventoryItemGridFragment>(InventoryItem, DkGameplayTags::Dk_Inventory_Fragment_Grid);
	const FInventoryItemImageFragment* ImageFragment =
		GetFragment<FInventoryItemImageFragment>(InventoryItem, DkGameplayTags::Dk_Inventory_Fragment_Icon);
	if (!GridFragment || !ImageFragment) return;

	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);

	DraggedItem->SetImageBrush(Brush);
	DraggedItem->SetGridDimension(GridFragment->GetGridSize());
	DraggedItem->SetInventoryItem(InventoryItem);
	DraggedItem->SetIsStackable(InventoryItem->IsItemStackable());
	DraggedItem->OnDraggedItemClicked.AddUniqueDynamic(this, &ThisClass::OnDraggedItemClicked);

	DraggedItem->SetDesiredSizeInViewport(Brush.ImageSize);
	DraggedItem->AddToViewport();
}

void UDKInventoryItemSpatialGrid::AssignDraggedItem(
	UDkInventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex)
{
	Super::AssignDraggedItem(InventoryItem, GridIndex, PreviousGridIndex);
}

void UDKInventoryItemSpatialGrid::UpdateTileParameters(
	const FVector2D& CanvasPosition, const FVector2D& MousePosition)
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
		const FInventoryItemGridFragment* GridFragment = GetFragment<FInventoryItemGridFragment>(
			CurrentSpaceQueryResult.ValidItem.Get(), DkGameplayTags::Dk_Inventory_Fragment_Grid
		);
		if (!GridFragment) return;

		ChangeHoverType(
			CurrentSpaceQueryResult.UpperLeftIndex,
			GridFragment->GetGridSize(),
			EInventoryGridSlotState::GrayedOut
		);

		// TODO: 如果这片区域内有一个Item，可以交换位置或者增加StackCount
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
				CurrentSlot->SetUnoccupiedTexture();
				break;
			case EInventoryGridSlotState::Occupied:
				CurrentSlot->SetOccupiedTexture();
				break;
			case EInventoryGridSlotState::Selected:
				CurrentSlot->SetSelectedTexture();
				break;
			case EInventoryGridSlotState::GrayedOut:
				CurrentSlot->SetGrayedOutTexture();
				break;
			}
		}
	);

	LastHighlightedIndex = StartingIndex,
		LastHighlightedDimension = Dimension;
}

bool UDKInventoryItemSpatialGrid::CursorExitedCanvas(
	const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& MousePos)
{
	bLastMouseWithInCanvas = bMouseWithInCanvas;
	bMouseWithInCanvas = UDkInventoryFunctionLibrary::IsWithInBounds(BoundaryPos, BoundarySize, MousePos);
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
			CurrentSlot->SetOccupiedTexture();
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
				CurrentSlot->SetUnoccupiedTexture();
			}
			else
			{
				CurrentSlot->SetOccupiedTexture();
			}
		}
	);
}

void UDKInventoryItemSpatialGrid::OnDraggedItemClicked(const FPointerEvent& MouseEvent)
{
	if (!IsValid(DraggedItem)) return;
	if (!GridSlots.IsValidIndex(ItemDropIndex)) return;

	if (CurrentSpaceQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentSpaceQueryResult.UpperLeftIndex))
	{
		OnSlottedItemClicked(CurrentSpaceQueryResult.UpperLeftIndex, MouseEvent);
		return;
	}

	UDkInventoryGridSlot* GridSlot = GridSlots[ItemDropIndex];
	if (!IsValid(GridSlot->GetInventoryItem()))
	{
		PutDownOnIndex(ItemDropIndex);
	}
}

void UDKInventoryItemSpatialGrid::PutDownOnIndex(const int32 Index)
{
	AddItemToIndex(DraggedItem->GetInventoryItem(), Index, DraggedItem->GetStackCount(), DraggedItem->GetIsStackable());
	UpdateGridSlots(DraggedItem->GetInventoryItem(), Index, DraggedItem->GetStackCount(),
	                DraggedItem->GetIsStackable());
	ClearDraggedItem();
}

void UDKInventoryItemSpatialGrid::ClearDraggedItem()
{
	if (!IsValid(DraggedItem)) return;

	DraggedItem->SetInventoryItem(nullptr);
	DraggedItem->SetIsStackable(false);
	DraggedItem->SetPreviousGridIndex(INDEX_NONE);
	DraggedItem->UpdateStackCount(0);
	DraggedItem->SetImageBrush(FSlateNoResource());

	DraggedItem->RemoveFromParent();
	DraggedItem = nullptr;
}
