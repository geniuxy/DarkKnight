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

void UDkInventoryEquipmentGridSlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text_Title->SetText(SlotTitle);
}
