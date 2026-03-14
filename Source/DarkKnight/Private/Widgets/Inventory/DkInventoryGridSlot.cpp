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

FVector2D UDkInventoryGridSlot::GetGridSlotSize() const
{
	return SizeBox_GridSlot->GetDesiredSize();
}

void UDkInventoryGridSlot::SetGridSlotSize(float InSlotSize)
{
	SizeBox_GridSlot->SetWidthOverride(InSlotSize);
	SizeBox_GridSlot->SetHeightOverride(InSlotSize);
}

void UDkInventoryGridSlot::SetUnoccupiedBrush()
{
	GridSlotState = EInventoryGridSlotState::Unoccupied;
	Image_GridSlot->SetBrush(UnoccupiedBrush);
}

void UDkInventoryGridSlot::SetOccupiedBrush()
{
	GridSlotState = EInventoryGridSlotState::Occupied;
	Image_GridSlot->SetBrush(OccupiedBrush);
}

void UDkInventoryGridSlot::SetEnabledBrush()
{
	GridSlotState = EInventoryGridSlotState::Enabled;
	Image_GridSlot->SetBrush(EnabledBrush);
}

void UDkInventoryGridSlot::SetDisabledBrush()
{
	GridSlotState = EInventoryGridSlotState::Disabled;
	Image_GridSlot->SetBrush(DisabledBrush);
}

void UDkInventoryGridSlot::SetGrayedOutBrush()
{
	GridSlotState = EInventoryGridSlotState::GrayedOut;
	Image_GridSlot->SetBrush(GrayedOutBrush);
}

UDkInventoryItem* UDkInventoryGridSlot::GetInventoryItem() const
{
	return InventoryItem.IsValid() ? InventoryItem.Get() : nullptr;
}

void UDkInventoryGridSlot::SetInventoryItem(UDkInventoryItem* InItem)
{
	InventoryItem = InItem;
}
