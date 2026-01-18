// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DKInventoryItemSpatialGrid.h"

#include "DkGameplayTags.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Widgets/Inventory/DkInventoryGridSlot.h"
#include "Widgets/Inventory/DkInventorySlottedItem.h"

struct FInventoryItemImageFragment;

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
	const int32 EndColumn = (StartIndex % Columns) + ItemDimension.X;
	const int32 EndRow = (StartIndex / Columns) + ItemDimension.Y;
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
	// 如果在一个并不储存
	if (const int32 UpperLeftIndex = GridSlot->GetStackCount(); UpperLeftIndex != INDEX_NONE)
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
		CheckedIndices.Append(TemporarilyClaimed);
		//   需要填充多少？
		const int32 AmountToFillInSlot =
			CalculateFillAmountForSlot(Result.bStackable, MaxStackCount, AmountToFill, GridSlot);
		if (AmountToFillInSlot == 0) continue;
		//   更新剩余待填充数量（AmountToFill）
		AmountToFill -= AmountToFillInSlot;
	}
	// 剩余量是多少？

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
			GridSlot->SetbAvailable(false);
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
		}
	}
}
