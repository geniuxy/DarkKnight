// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Equipment/DkInventoryEquipmentSlot.h"

#include "CommonLazyImage.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/OverlaySlot.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"

void UDkInventoryEquipmentSlot::SetBackGroundIcon(const TSoftObjectPtr<UTexture2D>& InTexture)
{
	Image_ItemIcon->SetBrushFromLazyTexture(InTexture);
	Image_ItemIcon->SetBrushTintColor(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));
}

void UDkInventoryEquipmentSlot::SetEquipmentIcon()
{
	if (!GetInventoryItem()) return;

	const FInventoryItemFragment_Image* ImageFragment =
		GetInventoryItem()->GetItemManifest().GetFragmentOfType<FInventoryItemFragment_Image>();

	Image_ItemIcon->SetBrushFromLazyTexture(ImageFragment->GetIcon());
}

FVector2D UDkInventoryEquipmentSlot::GetIconSize() const
{
	return GetGridSlotSize() - UWidgetLayoutLibrary::SlotAsOverlaySlot(Image_ItemIcon)->GetPadding().GetTopLeft();
}
