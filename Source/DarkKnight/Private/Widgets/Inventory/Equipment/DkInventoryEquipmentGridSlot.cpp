// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Equipment/DkInventoryEquipmentGridSlot.h"

#include "CommonTextBlock.h"
#include "Components/VerticalBox.h"
#include "Inventory/DkInventoryItem.h"
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

UDkInventoryItem* UDkInventoryEquipmentGridSlot::GetInventoryItem() const
{
	return EquipmentSlot->GetInventoryItem();
}

void UDkInventoryEquipmentGridSlot::UpdateEquipmentIcon(UDkInventoryItem* InItem)
{
	if (!IsValid(InItem)) return;
	EquipmentSlot->SetInventoryItem(InItem);
	EquipmentSlot->SetEquipmentIcon();
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

void UDkInventoryEquipmentGridSlot::SetDefaultBackGroundIcon()
{
	EquipmentSlot->SetBackGroundIcon(EquipmentBgIcon);
}

void UDkInventoryEquipmentGridSlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text_Title->SetText(SlotTitle);
	SetDefaultBackGroundIcon();
}
