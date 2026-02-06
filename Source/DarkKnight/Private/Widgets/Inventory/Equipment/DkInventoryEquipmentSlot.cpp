// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Equipment/DkInventoryEquipmentSlot.h"

#include "CommonLazyImage.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/OverlaySlot.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Widgets/Inventory/DkInventoryItemDescriptionMenu.h"

FReply UDkInventoryEquipmentSlot::NativeOnMouseButtonDown(
	const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	GridSlotClicked.Broadcast(TileIndex, InMouseEvent);

	if (IsValid(ItemDescriptionMenu))
	{
		ItemDescriptionMenu->RemoveFromParent();
		ItemDescriptionMenu = nullptr;
	}

	return FReply::Handled();
}

void UDkInventoryEquipmentSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	CreateItemDescriptionMenu();
}

void UDkInventoryEquipmentSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (IsValid(ItemDescriptionMenu))
	{
		ItemDescriptionMenu->RemoveFromParent();
		ItemDescriptionMenu = nullptr;
	}
}

void UDkInventoryEquipmentSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ItemDescriptionMenu && ItemDescriptionMenu->IsInViewport())
	{
		UDkUIFunctionLibrary::PositionWidgetAtMouse(
			ItemDescriptionMenu,
			FVector2D{8.f, 8.f},
			true,
			true,
			4
		);
	}
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

FVector2D UDkInventoryEquipmentSlot::GetIconSize() const
{
	return GetGridSlotSize() - UWidgetLayoutLibrary::SlotAsOverlaySlot(Image_EquipIcon)->GetPadding().GetTopLeft();
}

void UDkInventoryEquipmentSlot::CreateItemDescriptionMenu()
{
	if (!InventoryItem.IsValid()) return;

	if (!IsValid(ItemDescriptionMenu))
	{
		ItemDescriptionMenu = CreateWidget<UDkInventoryItemDescriptionMenu>(this, ItemDescriptionMenuClass);
	}

	// 根据Fragments，同化(渲染)ItemDescription的内容
	InventoryItem->GetItemManifest().AssimilateInventoryFragments(ItemDescriptionMenu);

	ItemDescriptionMenu->AddToViewport();
}
