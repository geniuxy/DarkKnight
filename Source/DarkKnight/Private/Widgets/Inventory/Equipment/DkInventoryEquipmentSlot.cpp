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

void UDkInventoryEquipmentSlot::SetBrush(FSlateBrush InBrush)
{
	Image_GridSlot->SetBrush(InBrush);
}
