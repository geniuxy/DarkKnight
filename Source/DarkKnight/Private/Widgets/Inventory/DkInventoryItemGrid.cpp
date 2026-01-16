// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryItemGrid.h"

#include "DarkKnightDebugHelper.h"
#include "Components/DkInventoryComponent.h"
#include "Widgets/Inventory/DkInventoryGridSlot.h"

#include "Components/UniformGridPanel.h"
#include "FunctionLibrarys/DkCommonFunctionLibrary.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"

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
}

void UDkInventoryItemGrid::AddItem(UDkInventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	Debug::Print(FString::Printf(
			TEXT("%s背包添加物品"),
			*UDkCommonFunctionLibrary::GetStringValueOfEnum<EInventoryItemCategory>(GetItemCategory()))
	);
}

bool UDkInventoryItemGrid::MatchesCategory(const UDkInventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
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

			GridSlot->SetGridSlotSize(TileSize);
			GridSlot->SetTileIndex(UDkInventoryFunctionLibrary::GetIndexFromPosition({i, j}, Columns));

			GridSlots.Add(GridSlot);
		}
	}
}
