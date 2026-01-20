// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryDraggedItem.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Inventory/DkInventoryItem.h"

void UDkInventoryDraggedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void UDkInventoryDraggedItem::UpdateStackCountText(const int32 Count) const
{
	if (Count > 0)
	{
		Text_StackCount->SetText(FText::AsNumber(Count));
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FGameplayTag UDkInventoryDraggedItem::GetItemTag() const
{
	if (InventoryItem.IsValid())
	{
		return InventoryItem->GetItemManifest().GetItemTag();
	}
	return FGameplayTag();
}

void UDkInventoryDraggedItem::SetIsStackable(bool bInIsStackable)
{
	bIsStackable = bInIsStackable;
	if (bInIsStackable)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UDkInventoryItem* UDkInventoryDraggedItem::GetInventoryItem() const
{
	return InventoryItem.IsValid() ? InventoryItem.Get() : nullptr;
}

void UDkInventoryDraggedItem::SetInventoryItem(UDkInventoryItem* InItem)
{
	InventoryItem = InItem;
}
