// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryGridSlot.h"

#include "Components/SizeBox.h"

void UDkInventoryGridSlot::SetGridSlotSize(float InSlotSize)
{
	SizeBox_GridSlot->SetWidthOverride(InSlotSize);
	SizeBox_GridSlot->SetHeightOverride(InSlotSize);  
}
