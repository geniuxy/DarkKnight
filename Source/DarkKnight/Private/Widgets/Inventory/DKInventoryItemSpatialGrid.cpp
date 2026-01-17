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

	// Store the new widget in a container.
}

FVector2D UDKInventoryItemSpatialGrid::GetDrawSize(const FInventoryItemGridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	return GridFragment->GetGridSize() * IconTileWidth;
}

void UDKInventoryItemSpatialGrid::ConstructGrid()
{
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
