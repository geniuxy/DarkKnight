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

void UDKInventoryItemSpatialGrid::UpdateGridSlots(UDkInventoryItem* NewItem, const int32 Index)
{
	check(GridSlots.IsValidIndex(Index));

	const FInventoryItemGridFragment* GridFragment =
		GetFragment<FInventoryItemGridFragment>(NewItem, DkGameplayTags::Dk_Inventory_Fragment_Grid);
	if (!GridFragment) return;

	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

	UDkInventoryFunctionLibrary::ForEach2D(
		GridSlots, Index, Dimensions, Columns,
		[](UDkInventoryGridSlot* GridSlot)
		{
			GridSlot->SetOccupiedTexture();
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
