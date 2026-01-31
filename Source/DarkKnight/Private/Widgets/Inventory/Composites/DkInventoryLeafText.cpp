// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Composites/DkInventoryLeafText.h"

#include "CommonTextBlock.h"

void UDkInventoryLeafText::SetText(const FText& Text) const
{
	LeafText->SetText(Text);
	LeafText->SetStyle(TextStyle);
}
