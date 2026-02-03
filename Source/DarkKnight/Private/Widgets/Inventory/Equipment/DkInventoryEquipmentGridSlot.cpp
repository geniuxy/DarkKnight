// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Equipment/DkInventoryEquipmentGridSlot.h"

#include "CommonTextBlock.h"
#include "Components/VerticalBox.h"
#include "Widgets/Inventory/Equipment/DkInventoryEquipmentSlot.h"

FVector2D UDkInventoryEquipmentGridSlot::GetTotalSlotSize() const
{
	return VertivalBox->GetDesiredSize();
}

FVector2D UDkInventoryEquipmentGridSlot::GetSlotSize() const
{
	return EquipmentSlot->GetGridSlotSize();
}

UDkInventoryEquipmentSlot* UDkInventoryEquipmentGridSlot::GetEquipmentSlot() const
{
	return EquipmentSlot;
}

void UDkInventoryEquipmentGridSlot::SetOccupiedBrush()
{
	EquipmentSlot->SetBrush(NormalBrush);
}

void UDkInventoryEquipmentGridSlot::SetEnabledBrush()
{
	EquipmentSlot->SetBrush(EnabledBrush);
}

void UDkInventoryEquipmentGridSlot::SetDisabledBrush()
{
	EquipmentSlot->SetBrush(DisabledBrush);
}

void UDkInventoryEquipmentGridSlot::SetGrayedOutBrush()
{
	EquipmentSlot->SetBrush(GrayedOutBrush);
}

void UDkInventoryEquipmentGridSlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text_Title->SetText(SlotTitle);
}
