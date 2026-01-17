// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventorySlottedItem.h"
#include "Inventory/DkInventoryItem.h"

#include "CommonLazyImage.h"

UDkInventoryItem* UDkInventorySlottedItem::GetInventoryItem() const
{
	return InventoryItem.IsValid() ? InventoryItem.Get() : nullptr;
}

void UDkInventorySlottedItem::SetInventoryItem(UDkInventoryItem* InItem)
{
	InventoryItem = InItem;
}

void UDkInventorySlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}
