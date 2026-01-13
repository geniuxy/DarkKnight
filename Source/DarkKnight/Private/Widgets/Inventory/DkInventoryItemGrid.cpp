// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryItemGrid.h"
#include "Widgets/Inventory/DkInventoryGridSlot.h"

#include "Components/UniformGridPanel.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"

void UDkInventoryItemGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GridPanel->SetMinDesiredSlotWidth(TileSize);
	GridPanel->SetMinDesiredSlotHeight(TileSize);
	GridPanel->SetSlotPadding(FMargin(SlotDistance));

	ConstructGrid();
}

void UDkInventoryItemGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns);

	for (int32 j = 0; j < Rows; ++j)
	{
		for (int32 i = 0; i < Columns; ++i)
		{
			UDkInventoryGridSlot* GridSlot = CreateWidget<UDkInventoryGridSlot>(this, GridSlotClass);
			GridPanel->AddChildToUniformGrid(GridSlot, j, i);

			GridSlot->SetTileIndex(UDkUIFunctionLibrary::GetIndexFromPosition({i,j}, Columns));

			GridSlots.Add(GridSlot);
		}
	}
}
