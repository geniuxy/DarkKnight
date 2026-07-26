// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryGridSlot.h"
#include "Inventory/DkInventoryItem.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Components/InventoryComps/DkInventoryComponent.h"
#include "Components/SizeBox.h"
#include "Components/InventoryComps/DkPlayerInventoryComp.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Widgets/Inventory/DkInventoryItemDescriptionMenu.h"

FReply UDkInventoryGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	GridSlotClicked.Broadcast(TileIndex, InMouseEvent);

	if (ItemDescriptionMenu.IsValid())
	{
		if (UDkPlayerInventoryComp* InventoryComponent =
			UDkInventoryFunctionLibrary::GetInventoryComponent(GetOwningPlayer()))
		{
			InventoryComponent->OnItemDescriptionMenuRemoved.Broadcast();
		}
		ItemDescriptionMenu = nullptr;
	}

	return FReply::Handled();
}

void UDkInventoryGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	GridSlotHovered.Broadcast(TileIndex, InMouseEvent);

	CreateItemDescriptionMenu();
}

void UDkInventoryGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	GridSlotUnhovered.Broadcast(TileIndex, InMouseEvent);

	if (ItemDescriptionMenu.IsValid())
	{
		if (UDkPlayerInventoryComp* InventoryComponent =
			UDkInventoryFunctionLibrary::GetInventoryComponent(GetOwningPlayer()))
		{
			InventoryComponent->OnItemDescriptionMenuRemoved.Broadcast();
		}
		ItemDescriptionMenu = nullptr;
	}
}

FVector2D UDkInventoryGridSlot::GetGridSlotSize() const
{
	return SizeBox_GridSlot->GetDesiredSize();
}

void UDkInventoryGridSlot::SetGridSlotSize(float InSlotSize)
{
	SizeBox_GridSlot->SetWidthOverride(InSlotSize);
	SizeBox_GridSlot->SetHeightOverride(InSlotSize);
}

void UDkInventoryGridSlot::SetUnoccupiedBrush()
{
	GridSlotState = EInventoryGridSlotState::Unoccupied;
	Image_GridSlot->SetBrush(UnoccupiedBrush);
}

void UDkInventoryGridSlot::SetOccupiedBrush()
{
	GridSlotState = EInventoryGridSlotState::Occupied;
	Image_GridSlot->SetBrush(OccupiedBrush);
}

void UDkInventoryGridSlot::SetEnabledBrush()
{
	GridSlotState = EInventoryGridSlotState::Enabled;
	Image_GridSlot->SetBrush(EnabledBrush);
}

void UDkInventoryGridSlot::SetDisabledBrush()
{
	GridSlotState = EInventoryGridSlotState::Disabled;
	Image_GridSlot->SetBrush(DisabledBrush);
}

void UDkInventoryGridSlot::SetGrayedOutBrush()
{
	GridSlotState = EInventoryGridSlotState::GrayedOut;
	Image_GridSlot->SetBrush(GrayedOutBrush);
}

void UDkInventoryGridSlot::SetDefaultItemIcon() const
{
	Image_ItemIcon->SetBrush(DefaultItemIconBrush);
}

void UDkInventoryGridSlot::SetItemIcon(const FSlateBrush& InBrush) const
{
	Image_ItemIcon->SetBrush(InBrush);
}

void UDkInventoryGridSlot::SetItemStackNum(int32 InStack)
{
	Text_StackNum->SetText(InStack == 0 ? FText::GetEmpty() : FText::AsNumber(InStack));
}

UDkInventoryItem* UDkInventoryGridSlot::GetInventoryItem() const
{
	return InventoryItem.IsValid() ? InventoryItem.Get() : nullptr;
}

void UDkInventoryGridSlot::SetInventoryItem(UDkInventoryItem* InItem)
{
	InventoryItem = InItem;
}

void UDkInventoryGridSlot::CreateItemDescriptionMenu()
{
	if (!bShouldCreateItemDescriptionMenu) return;
	if (!InventoryItem.IsValid()) return;

	if (!ItemDescriptionMenu.IsValid())
	{
		ItemDescriptionMenu = CreateWidget<UDkInventoryItemDescriptionMenu>(this, ItemDescriptionMenuClass);
	}

	// 根据Fragments，同化(渲染)ItemDescription的内容
	InventoryItem->GetItemManifest().AssimilateInventoryFragments(ItemDescriptionMenu.Get());

	if (UDkPlayerInventoryComp* InventoryComponent =
		UDkInventoryFunctionLibrary::GetInventoryComponent(GetOwningPlayer()))
	{
		InventoryComponent->OnItemDescriptionMenuCreated.Broadcast(ItemDescriptionMenu.Get());
	}
}
