// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryGridSlot.h"
#include "Inventory/DkInventoryItem.h"

#include "CommonLazyImage.h"
#include "Components/SizeBox.h"

void UDkInventoryGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	GridSlotHovered.Broadcast(TileIndex, InMouseEvent);
}

void UDkInventoryGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	GridSlotUnhovered.Broadcast(TileIndex, InMouseEvent);
}

void UDkInventoryGridSlot::SetGridSlotSize(float InSlotSize)
{
	SizeBox_GridSlot->SetWidthOverride(InSlotSize);
	SizeBox_GridSlot->SetHeightOverride(InSlotSize);
}

void UDkInventoryGridSlot::SetUnoccupiedTexture()
{
	GridSlotState = EInventoryGridSlotState::Unoccupied;
	Image_GridSlot->SetBrushFromLazyTexture(GridSlotBgImageUnoccupied);
}

void UDkInventoryGridSlot::SetOccupiedTexture()
{
	GridSlotState = EInventoryGridSlotState::Occupied;
	Image_GridSlot->SetBrushFromLazyTexture(GridSlotBgImageOccupied);
}

void UDkInventoryGridSlot::SetSelectedTexture()
{
	GridSlotState = EInventoryGridSlotState::Selected;
	Image_GridSlot->SetBrushFromLazyTexture(GridSlotBgImageSelected);
}

void UDkInventoryGridSlot::SetGrayedOutTexture()
{
	GridSlotState = EInventoryGridSlotState::GrayedOut;
	Image_GridSlot->SetBrushFromLazyTexture(GridSlotBgImageGrayedOut);
}

UDkInventoryItem* UDkInventoryGridSlot::GetInventoryItem() const
{
	return InventoryItem.IsValid() ? InventoryItem.Get() : nullptr;
}

void UDkInventoryGridSlot::SetInventoryItem(UDkInventoryItem* InItem)
{
	InventoryItem = InItem;
}
