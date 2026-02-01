// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Composites/DkInventoryLeafLabeledValue.h"

#include "CommonTextBlock.h"

void UDkInventoryLeafLabeledValue::SetTextLabel(const FText& Text, bool bCollapse) const
{
	if (bCollapse)
	{
		TextLabel->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	TextLabel->SetText(Text);
	TextLabel->SetStyle(TextLabelStyle);
}

void UDkInventoryLeafLabeledValue::SetTextValue(const FText& Text, bool bCollapse) const
{
	if (bCollapse)
	{
		TextValue->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	TextValue->SetText(Text);
	TextValue->SetStyle(TextValueStyle);
}
