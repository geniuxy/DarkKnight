// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Equipment/DkInventoryEquipmentSlot.h"

#include "CommonLazyImage.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"

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

void UDkInventoryEquipmentSlot::SetBackGroundIcon(const TSoftObjectPtr<UTexture2D>& InTexture)
{
	Image_EquipIcon->SetBrushFromLazyTexture(InTexture);
}

void UDkInventoryEquipmentSlot::SetEquipmentIcon()
{
	if (!GetInventoryItem()) return;

	const FInventoryItemImageFragment* ImageFragment =
		GetInventoryItem()->GetItemManifest().GetFragmentOfType<FInventoryItemImageFragment>();
	
	Image_EquipIcon->SetBrushFromTexture(ImageFragment->GetIcon());
}
