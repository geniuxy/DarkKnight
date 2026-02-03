// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Equipment/DkInventoryEquipmentSlot.h"

#include "CommonLazyImage.h"

void UDkInventoryEquipmentSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UDkInventoryEquipmentSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UDkInventoryEquipmentSlot::SetOccupiedTexture()
{
	GridSlotState = EInventoryGridSlotState::Occupied;
	Image_GridSlot->SetBrush(NormalBrush);
}

void UDkInventoryEquipmentSlot::SetEnabledBrush()
{
	GridSlotState = EInventoryGridSlotState::Enabled;
	Image_GridSlot->SetBrush(EnabledBrush);
}

void UDkInventoryEquipmentSlot::SetDisabledBrush()
{
	GridSlotState = EInventoryGridSlotState::Disabled;
	Image_GridSlot->SetBrush(DisabledBrush);
}

void UDkInventoryEquipmentSlot::SetGrayedOutTexture()
{
	GridSlotState = EInventoryGridSlotState::GrayedOut;
	Image_GridSlot->SetBrush(GrayedOutBrush);
}
