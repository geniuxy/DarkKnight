// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Equipment/DkInventoryEquipmentGridSlot.h"

#include "CommonTextBlock.h"
#include "Components/VerticalBox.h"
#include "Inventory/DkInventoryItem.h"
#include "Widgets/Inventory/Equipment/DkInventoryEquipmentSlot.h"

FVector2D UDkInventoryEquipmentGridSlot::GetTotalSlotSize() const
{
	return VerticalBox->GetDesiredSize();
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

void UDkInventoryEquipmentGridSlot::UpdateEquipmentInfo(UDkInventoryItem* InItem)
{
	if (!IsValid(InItem)) return;
	EquipmentSlot->SetInventoryItem(InItem);
	EquipmentSlot->SetEquipmentIcon();
}

void UDkInventoryEquipmentGridSlot::SetUnoccupiedBrush()
{
	EquipmentSlot->SetUnoccupiedBrush();
}

void UDkInventoryEquipmentGridSlot::SetEnabledBrush()
{
	EquipmentSlot->SetEnabledBrush();
}

void UDkInventoryEquipmentGridSlot::SetDisabledBrush()
{
	EquipmentSlot->SetDisabledBrush();
}

void UDkInventoryEquipmentGridSlot::SetGrayedOutBrush()
{
	EquipmentSlot->SetGrayedOutBrush();
}

void UDkInventoryEquipmentGridSlot::SetDefaultBackGroundIcon()
{
	EquipmentSlot->SetBackGroundIcon(EquipmentBgIcon);
}

void UDkInventoryEquipmentGridSlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text_Title->SetText(SlotTitle);
	if (!IsValid(GetInventoryItem()))
	{
		SetDefaultBackGroundIcon();
	}
}
