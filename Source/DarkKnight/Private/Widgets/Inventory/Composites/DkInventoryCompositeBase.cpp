// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Composites/DkInventoryCompositeBase.h"

void UDkInventoryCompositeBase::Collapse()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UDkInventoryCompositeBase::Expand()
{
	SetVisibility(ESlateVisibility::Visible);
}
